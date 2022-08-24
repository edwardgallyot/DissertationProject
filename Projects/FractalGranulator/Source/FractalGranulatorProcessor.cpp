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
    EdPF::AudioProcessor(CreateParameterLayout()),
    m_delayLine(88213)
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
    
}

void FractalGranulatorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{

    m_delayLine.WriteBlock(buffer.getNumSamples(), buffer);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        int delayTime = 44100;
        // Find the current phase according to the sample and writeIndex
        int index = m_delayLine.GetWriteIndex() - buffer.getNumSamples() + sample;
        // Subtract the Delay Time.
        index -= delayTime;
        if (index < 0)
        {
            // We don't want undefined behaviour from the modulo so we'll check it isn't negative
            index = m_delayLine.GetSize() + index;
        }
        else
        {
            // The index can be wrapped around the size so long as it is is positive.
            index = index % m_delayLine.GetSize();
        }
        // This can then be used to find a sample from the delay line.
        float newValue = m_delayLine.ReadSample(static_cast<float>(index));
        // We will add the samples to a copy buffer
        m_copyBuffer1.setSample(0, sample, newValue);
    }

    
    // The float vector operations will then add this to the current buffer.
    juce::FloatVectorOperations::add
    (
        buffer.getWritePointer(0, 0), 
        m_copyBuffer1.getReadPointer(0, 0), 
        buffer.getNumSamples()
    );

    buffer.clear(1, 0, buffer.getNumSamples());
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
            juce::NormalisableRange<float>(0.1f, 4000.0f),
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

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FractalGranulatorAudioProcessor();
}
