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
#include "../../EdPluginFramework/GUI/EdPF_AttachedButton.h"
#include "GUI/FractalInputComponent.h"
#include "GUI/FractalDisplayComponent.h"
#include "FractalGranulatorCommon.h"
#include "GUI/SliderLookAndFeel.h"

//==============================================================================
/**
*/
class FractalGranulatorAudioProcessorEditor  : public EdPF::AudioProcessorEditor
{
public:
    FractalGranulatorAudioProcessorEditor (FractalGranulatorAudioProcessor&);
    ~FractalGranulatorAudioProcessorEditor() override;

    void resized() override;
    void paint(juce::Graphics& g) override;
private:
	// Look and feel has to be instatiated first to avoid bad access.
    FGGUI::SliderLookAndFeel m_sliderLookAndFeel;


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
    EdPF::AttachedButton m_pitchQuantiseButton;
    EdPF::AttachedRotarySlider m_pitchShiftSlider;
    EdPF::AttachedRotarySlider m_harmonySlider;
     
    // Mix Slider
    EdPF::AttachedRotarySlider m_mixSlider;

    // Audio In Visualiser
    EdPF::AudioVisualiser m_audioInputVisualiser;
    
    // Audio Out Visualiser
    EdPF::AudioVisualiser m_audioOutputVisualiser; 

    // Fractal Input Component
    FGGUI::FractalInput m_fractalInput;

    // Fractal Display Component
    FGGUI::FractalDisplay m_fractalDisplay;
    

    FractalGranulatorAudioProcessor& m_processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FractalGranulatorAudioProcessorEditor)
};
