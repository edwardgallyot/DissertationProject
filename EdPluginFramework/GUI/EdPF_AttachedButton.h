/*
  ==============================================================================

    EdPF_AttachedButton.h
    Created: 11 Sep 2022 12:40:15pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_ATTACHED_BUTTON_H_INCLUDED
#define EDPF_ATTACHED_BUTTON_H_INCLUDED

#include <JuceHeader.h>
#include "../Utils/EdPF_AudioProcessor.h"

namespace EdPF
{
    class AttachedButton : public juce::ToggleButton
    {
    public:
        AttachedButton
    	(
            EdPF::AudioProcessor& p,
            juce::StringRef buttonID,
            juce::LookAndFeel* lookAndFeel
        );
        ~AttachedButton() = default;

    private:
        juce::ButtonParameterAttachment m_attachment;

    };
} //!EdPF

#endif  //!EDPF_ATTACHED_BUTTON_H_INCLUDED
