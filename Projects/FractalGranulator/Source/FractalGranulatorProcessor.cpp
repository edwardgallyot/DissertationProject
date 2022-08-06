/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "FractalGranulatorProcessor.h"
#include "FractalGranulatorEditor.h"

//==============================================================================
FractalGranulatorAudioProcessor::FractalGranulatorAudioProcessor()

{
}

FractalGranulatorAudioProcessor::~FractalGranulatorAudioProcessor()
{
}

void FractalGranulatorAudioProcessor::processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&)
{
    DBG("Block Processed!");
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
