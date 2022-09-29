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
    juce::Label m_inGainLabel;

    EdPF::AttachedRotarySlider m_outGainSlider;
    juce::Label m_outGainLabel;

    // Small left hand sliders
    EdPF::AttachedRotarySlider m_delayTimeSlider;
    juce::Label m_delayTimeLabel;
    EdPF::AttachedRotarySlider m_feedbackSlider;
    juce::Label m_feedbackLabel;

    // Small Right hand sliders
    EdPF::AttachedRotarySlider m_complexitySlider;
    juce::Label m_complexityLabel;
    EdPF::AttachedRotarySlider m_saturationSlider;
    juce::Label m_saturationLabel;

    // Shape Sliders
    EdPF::AttachedRotarySlider m_shape1Slider;
    juce::Label m_shape1Label;
    EdPF::AttachedRotarySlider m_shape2Slider;
    juce::Label m_shape2Label;
    EdPF::AttachedRotarySlider m_shape3Slider;
    juce::Label m_shape3Label;
    EdPF::AttachedRotarySlider m_shape4Slider;
    juce::Label m_shape4Label;

    // Right hand pitch sliders
    EdPF::AttachedButton m_pitchQuantiseButton;
    juce::Label m_quantiseLabel;
    EdPF::AttachedRotarySlider m_pitchShiftSlider;
    juce::Label m_pitchShiftLabel;
    EdPF::AttachedRotarySlider m_harmonySlider;
    juce::Label m_harmonyLabel;
     
    // Mix Slider
    EdPF::AttachedRotarySlider m_mixSlider;
    juce::Label m_mixLabel;

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
