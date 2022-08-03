/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGIN_EDITOR_H
#define PLUGIN_EDITOR_H

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SineWavePlot.h"

class SineWavePlotAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SineWavePlotAudioProcessorEditor (SineWavePlotAudioProcessor&);
    ~SineWavePlotAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Slider m_ampSlider;
    Slider m_freqSlider;
    SineWavePlot m_sineWavePlot;
    
    std::unique_ptr<SliderParameterAttachment> m_ampSliderAttachment;
    std::unique_ptr<SliderParameterAttachment> m_freqSliderAttachment;

    SineWavePlotAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SineWavePlotAudioProcessorEditor)
};

#endif //!PLUGIN_EDITOR_H
