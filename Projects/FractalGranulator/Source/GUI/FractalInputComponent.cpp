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
    g.setColour(FGConst::GrainColour);
    g.drawText("Fractal Granulator :)", getLocalBounds(), juce::Justification::centred); 
}
