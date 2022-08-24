/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FractalGranulatorProcessor.h"
#include "../../EdPluginFramework/Utils/EdPF_AudioProcessorEditor.h"
#include "../../EdPluginFramework/GUI/EdPF_AttatchedRotarySlider.h"
#include "../../EdPluginFramework/GUI/EdPF_AudioVisualiser.h"
#include "GUI/FractalInputComponent.h"
#include "GUI/FractalDisplayComponent.h"
#include "FractalGranulatorCommon.h"

//==============================================================================
/**
*/
class FractalGranulatorAudioProcessorEditor  : public EdPF::AudioProcessorEditor
{
public:
    FractalGranulatorAudioProcessorEditor (FractalGranulatorAudioProcessor&);
    ~FractalGranulatorAudioProcessorEditor() override;

    void resized() override;
private:
    // Sliders
    // In and Out Sliders
    EdPF::AttachedRotarySlider m_inGainSlider;
    EdPF::AttachedRotarySlider m_outGainSlider;

    // Small left hand sliders
    EdPF::AttachedRotarySlider m_delayTimeSlider;
    EdPF::AttachedRotarySlider m_spreadSlider;

    // Small Right hand sliders
    EdPF::AttachedRotarySlider m_complexitySlider;
    EdPF::AttachedRotarySlider m_saturationSlider;

    // Shape Sliders
    EdPF::AttachedRotarySlider m_shape1Slider;
    EdPF::AttachedRotarySlider m_shape2Slider;
    EdPF::AttachedRotarySlider m_shape3Slider;
    EdPF::AttachedRotarySlider m_shape4Slider;

    // Right hand pitch sliders
    EdPF::AttachedRotarySlider m_pitch1Slider;
    EdPF::AttachedRotarySlider m_pitch2Slider;
    EdPF::AttachedRotarySlider m_pitch3Slider;

    // Custom Components
    
    // Audio In Visualiser
    EdPF::AudioVisualiser m_audioInputVisualiser;
    
    // Audio Out Visualiser
    EdPF::AudioVisualiser m_audioOutputVisualiser;

    // TODO: Potentially put in an FFT Display

    // Fractal Input Component
    FGGUI::FractalInput m_fractalInput;

    // Fractal Display Component
    FGGUI::FractalDisplay m_fractalDisplay;

    FractalGranulatorAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FractalGranulatorAudioProcessorEditor)
};
