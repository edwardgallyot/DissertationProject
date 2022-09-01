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
    m_granulator(m_delayLine, GetSmoothedValuesBuffer(), m_currentPositionInfo),
    m_currentOutputMeter(0.0f)
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
    m_compressor.setThreshold(-6.0f);
    m_compressor.prepare(spec);


    // PREPARE THE FILTERS
    // ======================================================
    m_lowPass.prepare(sampleRate, expectedNumSamples, 2, juce::dsp::StateVariableFilter::StateVariableFilterType::lowPass);
    m_highPass.prepare(sampleRate, expectedNumSamples, 2, juce::dsp::StateVariableFilter::StateVariableFilterType::highPass);

    // PREPARE THE DISTORTION
    // =========================================================
    m_distortion.prepare(sampleRate, expectedNumSamples);
}

void FractalGranulatorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    // UPDATE PLAYHEAD INFO
    //==============================================================
    // The scheduler will use this to determine whether to schedule more grains
    m_currentPositionInfo.isPlaying = getPlayHead()->getPosition()->getIsPlaying();

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
        juce::FloatVectorOperations::multiply(buffer.getWritePointer(channel, 0), juce::Decibels::decibelsToGain(-12.0f), buffer.getNumSamples());
    }
    
    // APPLY INPUT GAIN
    //=================================================================
    auto inputGainBuffer = GetSmoothedValuesBuffer()[FGConst::Param_InGain].getReadPointer(0, 0);

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        juce::FloatVectorOperations::multiply(buffer.getWritePointer(channel, 0), inputGainBuffer, buffer.getNumSamples());
    }

    // CREATE FEEDBACK
    //================================================================== 
    auto inputRead = buffer.getReadPointer(0, 0);
    auto delayLineInput = m_delayInputBuffer.getWritePointer(0, 0);
    juce::FloatVectorOperations::copy(delayLineInput, inputRead, buffer.getNumSamples());
    auto feedbackOutputRead = m_feedbackOutputBuffer.getReadPointer(0, 0);
    juce::FloatVectorOperations::add(delayLineInput, feedbackOutputRead, buffer.getNumSamples());


    // WRITE TO OUR DELAY LINE
    // ================================================================
    m_delayLine.WriteBlock(m_delayInputBuffer.getNumSamples(), m_delayInputBuffer);

    // PROCESS THE GRANULATOR
    // ================================================================
    m_granulator.ProcessSamples(buffer);

    // UPDATE OUTPUT METER FOR GUI
    // ================================================================
    UpdateOutputValueMeter(buffer);

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
    
    // APPLY OUTPUT GAIN
    //================================================================
    auto outputGainBuffer = GetSmoothedValuesBuffer()[FGConst::Param_OutGain].getReadPointer(0, 0);

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        juce::FloatVectorOperations::multiply(buffer.getWritePointer(channel, 0), outputGainBuffer, buffer.getNumSamples());
    }

}

juce::AudioProcessorValueTreeState::ParameterLayout FractalGranulatorAudioProcessor::CreateParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Automated Parameters
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_InGain),
            "Input Gain",
            0.0f,
            1.0f,
            0.0f
        )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_OutGain),
            "Output Gain",
            0.0f,
            1.0f,
            0.0f
            )
    );

    auto delayRange = juce::NormalisableRange<float>(FGConst::DelayTimeMin, FGConst::DelayTimeMax);
    delayRange.setSkewForCentre(1000.0f);

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
            0.8f
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
            0.0f
            )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_Shape2),
            "Shape 2",
            0.0f,
            1.0f,
            0.0f
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
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_Pitch1),
            "Pitch 1",
            0.0f,
            1.0f,
            0.0f
            )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_Pitch2),
            "Pitch 2",
            0.0f,
            1.0f,
            0.0f
            )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_Pitch3),
            "Pitch 3",
            0.0f,
            1.0f,
            0.0f
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

void FractalGranulatorAudioProcessor::UpdateOutputValueMeter(juce::AudioBuffer<float>& buffer)
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
    m_currentOutputMeter.store(max);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FractalGranulatorAudioProcessor();
}
