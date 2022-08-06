/*
  ==============================================================================

    EdPF_AudioProcessorEditor.cpp
    Created: 3 Aug 2022 3:04:05pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "EdPF_AudioProcessorEditor.h"


EdPF::AudioProcessorEditor::AudioProcessorEditor(EdPF::AudioProcessor& p) 
    : juce::AudioProcessorEditor(p)
{

}

EdPF::AudioProcessorEditor::~AudioProcessorEditor() {}