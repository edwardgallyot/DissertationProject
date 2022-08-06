/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef FRACTAL_GRANULAR_PROCESSOR_H
#define FRACTAL_GRANULAR_PROCESSOR_H

#include <JuceHeader.h>
#include "../../EdPluginFramework/Utils/EdPF_AudioProcessorEditor.h"

class FractalGranulatorAudioProcessor  : public EdPF::AudioProcessor               
{
public:
    FractalGranulatorAudioProcessor();
    ~FractalGranulatorAudioProcessor() override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FractalGranulatorAudioProcessor)
};

#endif //!FRACTAL_GRANULAR_PROCESSOR_H