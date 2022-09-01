/*
  ==============================================================================

    EdPF_ToneModule.cpp
    Created: 30 Aug 2022 8:48:20pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "EdPF_ToneModule.h"

void EdPF::DSP::ToneModule::prepare (double sampleRate, unsigned int samplesPerBlock, unsigned int numChannels, StateVariableFilter::Parameters<float>::Type filterType)
{
    ProcessSpec processSpec{};
    processSpec.sampleRate = sampleRate;
    processSpec.maximumBlockSize = samplesPerBlock;
    processSpec.numChannels = numChannels;
    this->sampleRate = sampleRate;

    stateVariableFilter.reset();
    stateVariableFilter.prepare (processSpec);
    stateVariableFilter.state->type = filterType;
}
void EdPF::DSP::ToneModule::process (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages, float cutoff)
{
    AudioBlock<float> audioBlock(buffer);
    stateVariableFilter.state->setCutOffFrequency (sampleRate, cutoff, 0.5f);
    stateVariableFilter.process (ProcessContextReplacing<float> (audioBlock));
}

