/*
  ==============================================================================

    FractalDisplayComponent.cpp
    Created: 3 Aug 2022 2:50:49pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "FractalDisplayComponent.h"

void FGGUI::FractalDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::palegreen);
    g.drawText("Fractal Display", getLocalBounds(), juce::Justification::centred);
}
