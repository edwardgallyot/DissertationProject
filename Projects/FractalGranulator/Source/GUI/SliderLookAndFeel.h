/*
  ==============================================================================

    SliderLookAndFeel.h
    Created: 11 Sep 2022 6:29:38pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef SLIDER_LOOK_AND_FEEL_H_INCLUDED
#define SLIDER_LOOK_AND_FEEL_H_INCLUDED

#include <JuceHeader.h>
#include "../FractalGranulatorCommon.h"

namespace FGGUI
{
    class SliderLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
		SliderLookAndFeel() = default;
	    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height, float sliderPosProportional,
	                          float rotaryStartAngle, float rotaryEndAngle, juce::Slider&) override;
	    void drawButtonBackground(juce::Graphics&, juce::Button&, const juce::Colour& backgroundColour,
		    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
	    void drawToggleButton(juce::Graphics&, juce::ToggleButton&, bool shouldDrawButtonAsHighlighted,
		    bool shouldDrawButtonAsDown) override;
    };
}

#endif //!SLIDER_LOOK_AND_FEEL_H_INCLUDED