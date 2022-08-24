/*
  ==============================================================================

    EdPF_AttatchedRotarySlider.h
    Created: 3 Aug 2022 2:46:08pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_ATTACHED_ROTARY_SLIDER
#define EDPF_ATTACHED_ROTARY_SLIDER

#include <JuceHeader.h>
#include "../Utils/EdPF_AudioProcessor.h"

namespace EdPF
{
    class AttachedRotarySlider : public juce::Slider
    {
    public:
        AttachedRotarySlider
        (
            EdPF::AudioProcessor& p,
            juce::StringRef sliderID,
            juce::LookAndFeel* lookAndFeel
        );
        ~AttachedRotarySlider();
    
    private:
        std::unique_ptr<juce::SliderParameterAttachment> m_attachment;
    };
}

#endif //! EDPF_ATTATCHED_ROTARY_SLIDER
