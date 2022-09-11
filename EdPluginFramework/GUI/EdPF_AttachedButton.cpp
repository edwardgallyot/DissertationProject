/*
  ==============================================================================

    EdPF_AttachedButton.cpp
    Created: 11 Sep 2022 12:40:15pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "EdPF_AttachedButton.h"

EdPF::AttachedButton::AttachedButton
(
    EdPF::AudioProcessor& p,
    juce::StringRef buttonID,
    juce::LookAndFeel* lookAndFeel
) :
	ToggleButton(buttonID),
	m_attachment(*p.GetParameter(buttonID), *this, nullptr)
{
    setLookAndFeel(lookAndFeel);
}
