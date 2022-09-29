/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "FractalGranulatorProcessor.h"
#include "FractalGranulatorEditor.h"

//==============================================================================
FractalGranulatorAudioProcessorEditor::FractalGranulatorAudioProcessorEditor (FractalGranulatorAudioProcessor& p)
    : EdPF::AudioProcessorEditor (p), 
    m_processor (p), 
    m_inGainSlider(p, FGConst::GetParameterID(FGConst::Param_InGain), &m_sliderLookAndFeel),
    m_outGainSlider(p, FGConst::GetParameterID(FGConst::Param_OutGain), &m_sliderLookAndFeel),
    m_delayTimeSlider(p, FGConst::GetParameterID(FGConst::Param_DelayTime), &m_sliderLookAndFeel),
    m_feedbackSlider(p, FGConst::GetParameterID(FGConst::Param_Feedback), &m_sliderLookAndFeel),
    m_complexitySlider(p, FGConst::GetParameterID(FGConst::Param_Complexity), &m_sliderLookAndFeel),
    m_saturationSlider(p, FGConst::GetParameterID(FGConst::Param_Saturation), &m_sliderLookAndFeel),
    m_shape1Slider(p, FGConst::GetParameterID(FGConst::Param_Shape1), &m_sliderLookAndFeel),
    m_shape2Slider(p, FGConst::GetParameterID(FGConst::Param_Shape2), &m_sliderLookAndFeel),
    m_shape3Slider(p, FGConst::GetParameterID(FGConst::Param_Shape3), &m_sliderLookAndFeel),
    m_shape4Slider(p, FGConst::GetParameterID(FGConst::Param_Shape4), &m_sliderLookAndFeel),
    m_pitchQuantiseButton(p, FGConst::GetParameterID(FGConst::Param_PitchQuantize), &m_sliderLookAndFeel),
    m_pitchShiftSlider(p, FGConst::GetParameterID(FGConst::Param_PitchShift), &m_sliderLookAndFeel),
    m_harmonySlider(p, FGConst::GetParameterID(FGConst::Param_Harmony), &m_sliderLookAndFeel),
    m_mixSlider(p, FGConst::GetParameterID(FGConst::Param_Mix), &m_sliderLookAndFeel),
    m_fractalDisplay(p),
    m_audioInputVisualiser(p.GetInputWavePlotGenerator(), 50, 390, p.GetCurrentInputMeter()),
    m_audioOutputVisualiser(p.GetOuputWavePlotGenerator(), 50, 390, p.GetCurrentOutputMeter())
{
    setSize (1200, 400);

    addAndMakeVisible(m_inGainLabel);
    addAndMakeVisible(m_inGainSlider);
    addAndMakeVisible(m_outGainLabel);
    addAndMakeVisible(m_outGainSlider);
    addAndMakeVisible(m_delayTimeLabel);
    addAndMakeVisible(m_delayTimeSlider);
    addAndMakeVisible(m_feedbackLabel);
    addAndMakeVisible(m_feedbackSlider);
    addAndMakeVisible(m_complexityLabel);  
    addAndMakeVisible(m_complexitySlider);
    addAndMakeVisible(m_saturationLabel);
    addAndMakeVisible(m_saturationSlider);
    addAndMakeVisible(m_shape1Label);
    addAndMakeVisible(m_shape1Slider);
    addAndMakeVisible(m_shape2Label);
    addAndMakeVisible(m_shape2Slider);
    addAndMakeVisible(m_shape3Label);
    addAndMakeVisible(m_shape3Slider);
    addAndMakeVisible(m_shape4Label);
    addAndMakeVisible(m_shape4Slider);
    addAndMakeVisible(m_quantiseLabel);
    addAndMakeVisible(m_pitchQuantiseButton);
    addAndMakeVisible(m_pitchShiftLabel);
    addAndMakeVisible(m_pitchShiftSlider);
    addAndMakeVisible(m_harmonyLabel);
    addAndMakeVisible(m_harmonySlider);
    addAndMakeVisible(m_audioInputVisualiser);
    addAndMakeVisible(m_audioOutputVisualiser);
    addAndMakeVisible(m_fractalInput);
    addAndMakeVisible(m_fractalDisplay);
    addAndMakeVisible(m_mixLabel);
    addAndMakeVisible(m_mixSlider);
    setColour(juce::Label::textColourId, FGConst::OffWhiteColour);
}

FractalGranulatorAudioProcessorEditor::~FractalGranulatorAudioProcessorEditor()
{
}

void FractalGranulatorAudioProcessorEditor::resized()
{
    // Divide the screens up into thirds
    auto widthOverThree = static_cast<float>(getWidth()) / 3.0f;
    auto heightOverThree = static_cast<float>(getHeight()) / 3.0f;

    // The bottom left elements want to be a third of a third or a 9th square
    auto bottomLeftAreaTemplate = juce::Rectangle<float>(0.0f, heightOverThree * 2.0f, widthOverThree / 3.0f, heightOverThree);
    
    // Pass it to the inGain slider
    auto inGainArea = bottomLeftAreaTemplate;
    inGainArea.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_inGainSlider.setBounds(inGainArea.toNearestInt());
    inGainArea.setY(inGainArea.getY() + 50);
    inGainArea.setX(inGainArea.getX() + 38);
    m_inGainLabel.setText("Input", juce::dontSendNotification);
    m_inGainLabel.setBounds(inGainArea.toNearestInt());

    // Set the X coordinates and make mini sliders out of the delay and complexity
    auto delayTimeArea = bottomLeftAreaTemplate;
    delayTimeArea.setX((widthOverThree / 3.0f) + FGConst::GUIElementPadding);
    delayTimeArea.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    delayTimeArea.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_delayTimeSlider.setBounds(delayTimeArea.toNearestInt());
    delayTimeArea.setY(delayTimeArea.getY() + 40);
    delayTimeArea.setX(delayTimeArea.getX() + 24);
    m_delayTimeLabel.setText("Delay", juce::dontSendNotification);
    m_delayTimeLabel.setBounds(delayTimeArea.toNearestInt());

    // We can do the same for the spread area
    auto feedbackArea = bottomLeftAreaTemplate;
    feedbackArea.setX((widthOverThree / 3.0f) * 2.0f);
    feedbackArea.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    feedbackArea.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_feedbackSlider.setBounds(feedbackArea.toNearestInt());
    feedbackArea.setY(feedbackArea.getY() + 40);
    feedbackArea.setX(feedbackArea.getX() + 20);
    m_feedbackLabel.setText("Repeat", juce::dontSendNotification);
    m_feedbackLabel.setBounds(feedbackArea.toNearestInt());

    // We want to create 4 sliders in the bottom area so we set a template for this too and reduce it by the padding element
    auto bottomMiddleAreaTemplate = juce::Rectangle<float>(widthOverThree, heightOverThree * 2.0f, widthOverThree / 4.0f, heightOverThree);
    
    // We can then set the relevant x points for the 4 sliders
    auto shape1Bounds = bottomMiddleAreaTemplate;
    shape1Bounds.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_shape1Slider.setBounds(shape1Bounds.toNearestInt());
    shape1Bounds.setY(shape1Bounds.getY() + 40);
    shape1Bounds.setX(shape1Bounds.getX() + 18);
    m_shape1Label.setText("X Scale", juce::dontSendNotification);
    m_shape1Label.setBounds(shape1Bounds.toNearestInt());
    
    auto shape2Bounds = bottomMiddleAreaTemplate;
    shape2Bounds.setX(widthOverThree + (widthOverThree / 4.0f));
    shape2Bounds.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_shape2Slider.setBounds(shape2Bounds.toNearestInt());
    shape2Bounds.setY(shape2Bounds.getY() + 40);
    shape2Bounds.setX(shape2Bounds.getX() + 18);
    m_shape2Label.setText("Y Scale", juce::dontSendNotification);
    m_shape2Label.setBounds(shape2Bounds.toNearestInt());

    auto shape3Bounds = bottomMiddleAreaTemplate;
    shape3Bounds.setX(widthOverThree + ((widthOverThree / 4.0f) * 2.0f));
    shape3Bounds.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_shape3Slider.setBounds(shape3Bounds.toNearestInt());
    shape3Bounds.setY(shape3Bounds.getY() + 40);
    shape3Bounds.setX(shape3Bounds.getX() + 20);
    m_shape3Label.setText("Rotate", juce::dontSendNotification);
    m_shape3Label.setBounds(shape3Bounds.toNearestInt());

    auto shape4Bounds = bottomMiddleAreaTemplate;
    shape4Bounds.setX(widthOverThree + ((widthOverThree / 4.0f) * 3.0f));
    shape4Bounds.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_shape4Slider.setBounds(shape4Bounds.toNearestInt());
    shape4Bounds.setY(shape4Bounds.getY() + 40);
    shape4Bounds.setX(shape4Bounds.getX() + 18);
    m_shape4Label.setText("Random", juce::dontSendNotification);
    m_shape4Label.setBounds(shape4Bounds.toNearestInt());

    // The bottom right hand slider bank can be set up as the inverse of the left hand slider bank
    auto bottomRightAreaTemplate = juce::Rectangle<float>(widthOverThree * 2, heightOverThree * 2.0f, widthOverThree / 3.0f, heightOverThree);
    
    // We can pad this template area
    //bottomRightAreaTemplate.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);

    auto outGainArea = bottomRightAreaTemplate;
    outGainArea.setX(bottomRightAreaTemplate.getX() + ((widthOverThree / 3.0f) * 2));
    outGainArea.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_outGainSlider.setBounds(outGainArea.toNearestInt());
    outGainArea.setY(outGainArea.getY() + 50);
    outGainArea.setX(outGainArea.getX() + 35);
    m_outGainLabel.setText("Output", juce::dontSendNotification);
    m_outGainLabel.setBounds(outGainArea.toNearestInt());


    auto complexityArea = bottomRightAreaTemplate;
    complexityArea.setX(bottomRightAreaTemplate.getX());
    complexityArea.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    complexityArea.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_complexitySlider.setBounds(complexityArea.toNearestInt());
    complexityArea.setY(complexityArea.getY() + 40);
    complexityArea.setX(complexityArea.getX() + 8);
    m_complexityLabel.setText("Complexity", juce::dontSendNotification);
    m_complexityLabel.setBounds(complexityArea.toNearestInt());
    
    auto saturationArea = bottomRightAreaTemplate;
    saturationArea.setX(bottomRightAreaTemplate.getX() + (widthOverThree / 3.0f) - FGConst::GUIElementPadding);
    saturationArea.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    saturationArea.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_saturationSlider.setBounds(saturationArea.toNearestInt());
    saturationArea.setY(saturationArea.getY() + 40);
    saturationArea.setX(saturationArea.getX() + 22);
    m_saturationLabel.setText("Drive", juce::dontSendNotification);
    m_saturationLabel.setBounds(saturationArea.toNearestInt());

    // Pitch Slider Positioning

    auto twoThirdsHeightOfLeftThird =
        juce::Rectangle<float>
        (
            /*X*/0,
            /*Y*/0,
            /*WIDTH*/widthOverThree,
            /*HEIGHT*/heightOverThree * 2.0f
            );

    auto centreRightAreaTemplate = 
        juce::Rectangle<float>
        (
            /*X*/widthOverThree * 2,
            /*Y*/(twoThirdsHeightOfLeftThird.getHeight() / 3.0f),
            /*WIDTH*/widthOverThree / 3.0f,
            /*HEIGHT*/twoThirdsHeightOfLeftThird.getHeight() / 3.0f * 2.0f
        );

    auto quantiseArea = centreRightAreaTemplate;
    quantiseArea.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    quantiseArea.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_pitchQuantiseButton.setBounds(quantiseArea.toNearestInt());
    quantiseArea.setY(quantiseArea.getY() + 25);
    quantiseArea.setX(quantiseArea.getX() + 5);
    m_quantiseLabel.setText("Pitch Quantise", juce::dontSendNotification);
    m_quantiseLabel.setBounds(quantiseArea.toNearestInt());

    auto pitchShiftArea = centreRightAreaTemplate;
    pitchShiftArea.setX((widthOverThree * 2.0f) + (widthOverThree / 3.0f));
    pitchShiftArea.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    pitchShiftArea.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_pitchShiftSlider.setBounds(pitchShiftArea.toNearestInt());
    pitchShiftArea.setY(pitchShiftArea.getY() + 43);
    pitchShiftArea.setX(pitchShiftArea.getX() + 10);
    m_pitchShiftLabel.setText("Pitch Shift", juce::dontSendNotification);
    m_pitchShiftLabel.setBounds(pitchShiftArea.toNearestInt());

    auto harmonyArea = centreRightAreaTemplate;
    harmonyArea.setX((widthOverThree * 2.0f) + (2.0f * (widthOverThree / 3.0f)));
    harmonyArea.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    harmonyArea.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_harmonySlider.setBounds(harmonyArea.toNearestInt());
    harmonyArea.setY(harmonyArea.getY() + 43);
    harmonyArea.setX(harmonyArea.getX() + 14);
    m_harmonyLabel.setText("Harmony", juce::dontSendNotification);
    m_harmonyLabel.setBounds(harmonyArea.toNearestInt());
    
    // Audio Output Component
    auto audioOuputBounds = twoThirdsHeightOfLeftThird;
    audioOuputBounds.setX(widthOverThree * 2.0f);
    audioOuputBounds.removeFromBottom((twoThirdsHeightOfLeftThird.getHeight() / 3.0f) * 2.0f);
    audioOuputBounds.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_audioOutputVisualiser.setBounds(audioOuputBounds.toNearestInt());


    // Audio input visualiser bounds
    auto audioInputBounds = twoThirdsHeightOfLeftThird;
    auto currentSectionOverThree = twoThirdsHeightOfLeftThird.getHeight() / 3.0f;
    audioInputBounds.removeFromBottom(currentSectionOverThree * 2.0f);
    audioInputBounds.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_audioInputVisualiser.setBounds(audioInputBounds.toNearestInt());

    // Fractal Input Bounds
    auto fractalInputBounds = twoThirdsHeightOfLeftThird;
    fractalInputBounds.removeFromTop(currentSectionOverThree);
    fractalInputBounds.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    fractalInputBounds.removeFromRight(static_cast<float>(fractalInputBounds.getWidth()) / 3.0f);
    m_fractalInput.setBounds(fractalInputBounds.toNearestInt());

    // Mix Slider
    auto mixSliderBounds = twoThirdsHeightOfLeftThird;
    mixSliderBounds.removeFromTop(currentSectionOverThree);
    mixSliderBounds.removeFromLeft(2.0f * static_cast<float>(mixSliderBounds.getWidth()) / 3.0f);
    mixSliderBounds.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    mixSliderBounds.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    m_mixSlider.setBounds(mixSliderBounds.toNearestInt());
    mixSliderBounds.setY(mixSliderBounds.getY() + 40);
    mixSliderBounds.setX(mixSliderBounds.getX() + 30);
    m_mixLabel.setText("Mix", juce::dontSendNotification);
    m_mixLabel.setBounds(mixSliderBounds.toNearestInt());

    // Fractal Display Bounds
    auto twoThirdsHeightOfMiddleThird = twoThirdsHeightOfLeftThird;
    twoThirdsHeightOfMiddleThird.setX(widthOverThree); 
    twoThirdsHeightOfMiddleThird.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_fractalDisplay.setBounds(twoThirdsHeightOfMiddleThird.toNearestInt());
}

void FractalGranulatorAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(FGConst::BackgroundColour);
}
