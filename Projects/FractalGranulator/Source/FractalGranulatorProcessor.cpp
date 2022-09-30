/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "FractalGranulatorProcessor.h"
#include "FractalGranulatorEditor.h"

//==============================================================================
FractalGranulatorAudioProcessor::FractalGranulatorAudioProcessor()
    :
    EdPF::AudioProcessor(CreateParameterLayout(), FGConst::NumOfParams),
    m_delayLine(0),
    m_granulator(*this, m_delayLine, GetSmoothedValuesBuffer()),
    m_currentOutputMeter(0.0f),
    m_inputWaveGenerator(1000.0f, 390),
    m_outputWaveGenerator(1000.0f, 390)
{
}

FractalGranulatorAudioProcessor::~FractalGranulatorAudioProcessor()
{
}

void FractalGranulatorAudioProcessor::prepareToPlay(double sampleRate, int expectedNumSamples)
{
    // PREPARE COPY BUFFERS
    // ==================================================
    m_feedbackOutputBuffer.setSize(1, expectedNumSamples);
    m_feedbackOutputBuffer.clear();
    m_delayInputBuffer.setSize(1, expectedNumSamples);
    m_dryBuffer.setSize(2, expectedNumSamples);

    // SET VALUE SMOOTHING
    //=====================================================
    PrepareSmoothedValues(sampleRate, expectedNumSamples);

    // PREPARE GRANULATOR
    //======================================================
    m_granulator.PrepareToPlay(sampleRate, expectedNumSamples);
    

    // PREPARE THE DELAY LINE
    // =====================================================
    m_delayLine.SetSize
    (
        4 * static_cast<int>(EdPF::DSP::Utils::MsToSamples(FGConst::DelayTimeMax, static_cast<float>(sampleRate)))
    );

    // PREPARE THE COMPRESSOR
    // ======================================================
    juce::dsp::ProcessSpec spec{};
    spec.sampleRate = sampleRate;
    spec.numChannels = 2;
    spec.sampleRate = sampleRate;

    m_compressor.reset();
    m_compressor.setAttack(5.0f);
    m_compressor.setRelease(100.0f);
    m_compressor.setRatio(4.0f);
    m_compressor.setThreshold(0.0f);
    m_compressor.prepare(spec);


    // PREPARE THE FILTERS
    // ======================================================
    m_lowPass.prepare(sampleRate, expectedNumSamples, 2, juce::dsp::StateVariableFilter::StateVariableFilterType::lowPass);
    m_highPass.prepare(sampleRate, expectedNumSamples, 2, juce::dsp::StateVariableFilter::StateVariableFilterType::highPass);

    // PREPARE THE DISTORTION
    // =========================================================
    m_distortion.prepare(sampleRate, expectedNumSamples);

    // PREPARE THE PLOT GENERATORS
    // =========================================================
    m_inputWaveGenerator.SetSampleRate(sampleRate);
    m_outputWaveGenerator.SetSampleRate(sampleRate);
}

void FractalGranulatorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    // TODO: Support Stereo Configurations
    
    // UPDATE SMOOTHED SAMPLES
    //================================================================
    for (int i = 0; i < FGConst::NumOfParams; ++i)
    {
        UpdateSmoothedValues
        (
            i,
            static_cast<float>(GetParameterAsValue(FGConst::GetParameterID(static_cast<FGConst::Params>(i))).getValue())
        );
    }
    CopySmoothedValuesToBuffers();


    // ATTENUATE INPUT
    // ===============================================================
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        juce::FloatVectorOperations::multiply(buffer.getWritePointer(channel, 0), juce::Decibels::decibelsToGain(-3.0f), buffer.getNumSamples());
    }
    
    // APPLY INPUT GAIN
    //=================================================================
    const auto inputGainBuffer = GetSmoothedValuesBuffer()[FGConst::Param_InGain].getReadPointer(0, 0);

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        juce::FloatVectorOperations::multiply(buffer.getWritePointer(channel, 0), inputGainBuffer, buffer.getNumSamples());
    }

    // COPY DRY INPUT TO BUFFER
    // ================================================================
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        juce::FloatVectorOperations::copy(m_dryBuffer.getWritePointer(channel, 0), buffer.getReadPointer(channel, 0), buffer.getNumSamples());
    }

    // PROCESS INPUT FIFO
    // ================================================================
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        m_inputWaveGenerator.ProcessSample(buffer.getSample(0, i));
    }

    // UPDATE INPUT METER
    // ===================================================================
    UpdateSimpleMeter(&m_currentInputMeter, buffer);

    // CREATE FEEDBACK
    //================================================================== 
    const auto inputRead = buffer.getReadPointer(0, 0);
    const auto delayLineInput = m_delayInputBuffer.getWritePointer(0, 0);
    juce::FloatVectorOperations::copy(delayLineInput, inputRead, buffer.getNumSamples());
    const auto feedbackOutputRead = m_feedbackOutputBuffer.getReadPointer(0, 0);
    juce::FloatVectorOperations::add(delayLineInput, feedbackOutputRead, buffer.getNumSamples());


    // WRITE TO OUR DELAY LINE
    // ================================================================
    m_delayLine.WriteBlock(m_delayInputBuffer.getNumSamples(), m_delayInputBuffer);

    // PROCESS THE GRANULATOR
    // ================================================================
    m_granulator.ProcessSamples(buffer);

    // UPDATE GRANULATOR METER FOR GUI
    // ================================================================
    UpdateSimpleMeter(&m_granulationMeter,buffer);

    // COMPRESS THE OUTPUT
    // ==============================================================
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto out = m_compressor.processSample(0, buffer.getSample(channel, sample));
            buffer.setSample(channel, sample, out);
        }
    }

    // FILTER THE OUTPUT
    // ================================================================
    m_highPass.process(buffer, midi, 40.0f);
    m_lowPass.process(buffer, midi, 19000.0f);


    // COPY THE OUTPUT TO OUR FEEDBACK BUFFER
    // ================================================================
    auto outLPtr = buffer.getReadPointer(0, 0);
    auto feedbackOutputWrite = m_feedbackOutputBuffer.getWritePointer(0, 0);
    auto feedbackAmountPtr = GetSmoothedValuesBuffer()[FGConst::Param_Feedback].getReadPointer(0, 0);
    juce::FloatVectorOperations::copy(feedbackOutputWrite, outLPtr, buffer.getNumSamples());
    juce::FloatVectorOperations::multiply(feedbackOutputWrite, feedbackAmountPtr, buffer.getNumSamples());

    // DISTORT THE OUTPUT
    // ================================================================
    auto drive = GetSmoothedValuesBuffer()[FGConst::Param_Saturation].getSample(0, 0);
    m_distortion.process(buffer, midi, drive);
    

    // ADD IN THE DRY SIGNAL
    // ============================================================================
    auto mixbuffer = GetSmoothedValuesBuffer()[FGConst::Param_Mix].getReadPointer(0, 0);
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        juce::FloatVectorOperations::multiply(buffer.getWritePointer(channel, 0), mixbuffer, buffer.getNumSamples());
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            m_dryBuffer.setSample(channel, i, m_dryBuffer.getSample(channel, i) * (1.0f - mixbuffer[i]));
        }
    }
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        juce::FloatVectorOperations::add(buffer.getWritePointer(channel, 0), m_dryBuffer.getReadPointer(channel, 0), buffer.getNumSamples());
    }
    
    // APPLY OUTPUT GAIN
    //================================================================
    auto outputGainBuffer = GetSmoothedValuesBuffer()[FGConst::Param_OutGain].getReadPointer(0, 0);
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        juce::FloatVectorOperations::multiply(buffer.getWritePointer(channel, 0), outputGainBuffer, buffer.getNumSamples());
    }

    // UPDATE OUTPUT METER
    UpdateSimpleMeter(&m_currentOutputMeter, buffer);

    // PROCESS OUTPUT FIFO
    // ================================================================
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        m_outputWaveGenerator.ProcessSample(buffer.getSample(0, i));
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout FractalGranulatorAudioProcessor::CreateParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Automated Parameters
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_Mix),
            "Mix",
            0.0f,
            1.0f,
            1.0f
            )
    );

    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_InGain),
            "Input Gain",
            0.0f,
            1.0f,
            0.5f
        )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_OutGain),
            "Output Gain",
            0.0f,
            1.0f,
            0.5f
            )
    );

    auto delayRange = juce::NormalisableRange<float>(FGConst::DelayTimeMin, FGConst::DelayTimeMax);
    delayRange.setSkewForCentre(500.0f);

    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_DelayTime),
            "Delay Time",
            delayRange,
            0.0f
            )
    );

    auto feedbackRange = juce::NormalisableRange<float>(0, juce::Decibels::decibelsToGain(2.0f) - 1.0f, 0.0000001f);
    feedbackRange.setSkewForCentre(0.1f);
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_Feedback),
            "Feedback",
            feedbackRange,
            0.0f
            )
    );

    auto complexityRange = juce::NormalisableRange<float>(0.5f, 1.0f, 0.000001f);
    complexityRange.setSkewForCentre(0.8f);

    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_Complexity),
            "Complexity",
            complexityRange,
            1.0f
            )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_Saturation),
            "Saturation",
            0.0f,
            1.0f,
            0.0f
            )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_Shape1),
            "Shape 1",
            0.0f,
            1.0f,
            0.5f
            )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_Shape2),
            "Shape 2",
            0.0f,
            1.0f,
            0.5f
            )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_Shape3),
            "Shape 3",
            0.0f,
            1.0f,
            0.0f
            )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_Shape4),
            "Shape 4",
            0.0f,
            1.0f,
            0.0f
            )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterBool>
        (
            FGConst::GetParameterID(FGConst::Param_PitchQuantize),
            "Pitch Quantize",
            false
            )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterInt>
        (
            FGConst::GetParameterID(FGConst::Param_PitchShift),
            "Pitch Shift",
            -12,
            12,
            0
            )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterInt>
        (
            FGConst::GetParameterID(FGConst::Param_Harmony),
            "Pitch Harmony",
            FGConst::Harmony::First,
            FGConst::Harmony::NumKeys - 1,
            0
            // Attributes
        )
    );

    // Non-Automatable Parameters can be like 
    //juce::AudioParameterFloatAttributes().withAutomatable(false)
    return {params.begin(), params.end()};
}

juce::AudioProcessorEditor* FractalGranulatorAudioProcessor::createEditor()
{
    return new FractalGranulatorAudioProcessorEditor(*this);
}

void FractalGranulatorAudioProcessor::UpdateSimpleMeter(std::atomic<float>* meter, juce::AudioBuffer<float>& buffer)
{
    float max = 0.0f;
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float currentSample = buffer.getSample(channel, sample);
            currentSample = std::abs(currentSample);
            max = std::max(max, currentSample);
        }
    }
    meter->store(max);
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FractalGranulatorAudioProcessor();
}
