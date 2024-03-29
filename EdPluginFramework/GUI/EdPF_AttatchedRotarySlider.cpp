/*
  ==============================================================================

    EdPF_AttatchedRotarySlider.cpp
    Created: 3 Aug 2022 2:46:08pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "EdPF_AttatchedRotarySlider.h"

EdPF::AttachedRotarySlider::AttachedRotarySlider
    (
        EdPF::AudioProcessor& p,
        juce::StringRef sliderID,
        juce::LookAndFeel_V4* lookAndFeel
    ) 
    :
    m_attachment(*p.GetParameter(sliderID), *this, nullptr)   
{
    setTextBoxStyle(NoTextBox, true, 0, 0);
    setSliderStyle(SliderStyle::RotaryHorizontalVerticalDrag);
    setLookAndFeel(lookAndFeel);
}

