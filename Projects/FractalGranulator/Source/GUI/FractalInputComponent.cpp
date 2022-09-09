/*
  ==============================================================================

    FractalInputComponent.cpp
    Created: 20 Aug 2022 9:25:40am
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "FractalInputComponent.h"

void FGGUI::FractalInput::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::blueviolet);
    g.drawText("LOGO!", getLocalBounds(), juce::Justification::centred); 
}
