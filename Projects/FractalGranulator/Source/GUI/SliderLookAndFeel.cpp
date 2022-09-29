/*
  ==============================================================================

    SliderLookAndFeel.cpp
    Created: 11 Sep 2022 6:29:38pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "SliderLookAndFeel.h"

void FGGUI::SliderLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider&)
{
    auto radius = static_cast<float>(juce::jmin(width / 2, height / 2)) - 4.0f;
    auto centreX = static_cast<float>(x) + static_cast<float>(width) * 0.5f;
    auto centreY = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    auto thickness = 0.85f;

    auto innerCircleX = centreX - (0.4f * radius);
    auto innerCircleY = centreY - (0.4f * radius);


    g.setColour(FGConst::GrainColour);
    g.fillEllipse(innerCircleX, innerCircleY, rw * 0.4f, rw * 0.4f);



    g.setColour(FGConst::ControlColour1);
    juce::Path p;
    auto pointerLength = radius * 0.1f;
    auto pointerThickness = 8.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius * 0.5f, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    g.fillPath(p);
  
    {
        g.setColour(FGConst::ControlColour2);
        juce::Path filledArc;
        filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, thickness);
        g.fillPath(filledArc);
    }
    

	{
        g.setColour(FGConst::WaveFormColour);
		juce::Path filledArc;
    	filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, angle, thickness);
    	g.fillPath(filledArc);
	}
}

void FGGUI::SliderLookAndFeel::drawButtonBackground(juce::Graphics&, juce::Button&, const juce::Colour& /*backgroundColour*/, bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/)
{
    
}

void FGGUI::SliderLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
	bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/)
{
    if (static_cast<bool>(button.getToggleStateValue().getValue()))
    {
        g.setColour(FGConst::GrainColour);
    }
    else
    {
        g.setColour(FGConst::OffWhiteColour);
    }

    auto bounds = button.getLocalBounds().toFloat().reduced(button.getWidth() / 3.0f, button.getHeight() / 3.0f);
    bounds.setHeight(bounds.getWidth());
	g.fillRoundedRectangle(bounds, 2.0f);

    if (static_cast<bool>(button.getToggleStateValue().getValue()))
    {
        g.setColour(FGConst::ControlColour1);
    }
    else
    {
        g.setColour(FGConst::WaveFormColour);
    }
    
    g.drawRoundedRectangle(bounds, 2.0f, 1.0f);
}


