/*
  ==============================================================================

    FractalInputComponent.h
    Created: 20 Aug 2022 9:25:40am
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef FRACTAL_INPUT_COMPONENT_H
#define FRACTAL_INPUT_COMPONENT_H

#include <JuceHeader.h>
#include "../FractalGranulatorCommon.h"

namespace FGGUI
{
    class FractalInput : public juce::Component
    {
        void paint(juce::Graphics& g) override;
    };
}

#endif //!FRACTAL_INPUT_COMPONENT_H
