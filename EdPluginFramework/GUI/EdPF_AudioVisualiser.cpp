/*
  ==============================================================================

    EdPF_AudioVisualiser.cpp
    Created: 20 Aug 2022 9:28:28am
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "EdPF_AudioVisualiser.h"

void EdPF::AudioVisualiser::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::peachpuff);
    g.drawText("Audio Visualise!", getLocalBounds(), juce::Justification::centred);
}
