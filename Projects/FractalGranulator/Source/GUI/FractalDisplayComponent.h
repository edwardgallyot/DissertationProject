/*
  ==============================================================================

    FractalDisplayComponent.h
    Created: 3 Aug 2022 2:50:49pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef FRACTAL_DISPLAY_COMPONENT_H
#define FRACTAL_DISPLAY_COMPONENT_H

#include "JuceHeader.h"

namespace FGGUI
{
    class FractalDisplay : public juce::Component 
    {
        void paint(juce::Graphics& g);
    };
} 

#endif //!FRACTAL_DISPLAY_COMPONENT_H
