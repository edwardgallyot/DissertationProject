/*
  ==============================================================================

    EdPF_AudioProcessorEditor.h
    Created: 3 Aug 2022 3:04:05pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_AUDIO_PROCESSOR_EDITOR_H
#define EDPF_AUDIO_PROCESSOR_EDITOR_H

#include <JuceHeader.h>
#include "../Utils/EdPF_AudioProcessor.h"

namespace EdPF 
{
    class AudioProcessorEditor : public juce::AudioProcessorEditor
    {
    public:
        AudioProcessorEditor(EdPF::AudioProcessor& p);
        ~AudioProcessorEditor();   
    };
}

#endif // !EDPF_AUDIO_PROCESSOR_EDITOR_H