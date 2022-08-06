/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FractalGranulatorProcessor.h"
#include "../../EdPluginFramework/Utils/EdPF_AudioProcessorEditor.h"

//==============================================================================
/**
*/
class FractalGranulatorAudioProcessorEditor  : public EdPF::AudioProcessorEditor
{
public:
    FractalGranulatorAudioProcessorEditor (FractalGranulatorAudioProcessor&);
    ~FractalGranulatorAudioProcessorEditor() override;
private:
    FractalGranulatorAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FractalGranulatorAudioProcessorEditor)
};
