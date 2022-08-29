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
    // Reset the Copy Buffer
    m_copyBuffer1.setSize(1, expectedNumSamples);
    m_copyBuffer1.clear();

    PrepareSmoothedValues(sampleRate, expectedNumSamples);
    m_granulator.PrepareToPlay(sampleRate, expectedNumSamples);
    m_delayLine.SetSize
    (
        4 * static_cast<int>(EdPF::DSP::Utils::MsToSamples(FGConst::DelayTimeMax, static_cast<float>(sampleRate)))
    );
}

void FractalGranulatorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    // UPDATE PLAYHEAD INFO
    //==============================================================
    // The scheduler will use this to determine whether to schedule more grains
    getPlayHead()->getCurrentPosition(m_currentPositionInfo);
    
    
    //================================================================
    // UPDATE SMOOTHED SAMPLES
    for (int i = 0; i < FGConst::NumOfParams; ++i)
    {
        UpdateSmoothedValues
        (
            i,
            static_cast<float>(GetParameterAsValue(FGConst::GetParameterID(static_cast<FGConst::Params>(i))).getValue())
        );
    }
    CopySmoothedValuesToBuffers();
    
    // ================================================================
    // WRITE TO OUR DELAY LINE
    m_delayLine.WriteBlock(buffer.getNumSamples(), buffer);

    // ================================================================
    // PROCESS THE GRANULATOR
    m_granulator.ProcessSamples(buffer);

    // UPDATE OUTPUT METER FOR GUI
    // ================================================================
    UpdateOutputValueMeter(buffer);

    
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
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_DelayTime),
            "Delay Time",
            juce::NormalisableRange<float>(FGConst::DelayTimeMin, FGConst::DelayTimeMax),
            0.0f
            )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_Spread),
            "Spread",
            0.0f,
            1.0f,
            0.0f
            )
    );
    params.push_back
    (std::make_unique<juce::AudioParameterFloat>
        (
            FGConst::GetParameterID(FGConst::Param_Complexity),
            "Complexity",
            0.0f,
            1.0f,
            0.0f
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
    m_currentOutputMeter.setValue(max);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FractalGranulatorAudioProcessor();
}
