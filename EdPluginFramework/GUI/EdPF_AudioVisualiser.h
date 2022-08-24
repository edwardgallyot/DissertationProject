/*
  ==============================================================================

    EdPF_AudioVisualiser.h
    Created: 20 Aug 2022 9:28:28am
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_AUDIO_VISUALISER_H 
#define EDPF_AUDIO_VISUALISER_H

#include "JuceHeader.h"

namespace EdPF
{
    class AudioVisualiser : public juce::Component
    {
        void paint(juce::Graphics& g) override;
    };
}

#endif //!EDPF_AUDIO_VISUALISER_H
