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
    audioProcessor (p), 
    m_inGainSlider(p, FGConst::GetParameterID(FGConst::Param_InGain), nullptr),
    m_outGainSlider(p, FGConst::GetParameterID(FGConst::Param_OutGain), nullptr),
    m_delayTimeSlider(p, FGConst::GetParameterID(FGConst::Param_DelayTime), nullptr),
    m_spreadSlider(p, FGConst::GetParameterID(FGConst::Param_Feedback), nullptr),
    m_complexitySlider(p, FGConst::GetParameterID(FGConst::Param_Complexity), nullptr),
    m_saturationSlider(p, FGConst::GetParameterID(FGConst::Param_Saturation), nullptr),
    m_shape1Slider(p, FGConst::GetParameterID(FGConst::Param_Shape1), nullptr),
    m_shape2Slider(p, FGConst::GetParameterID(FGConst::Param_Shape2), nullptr),
    m_shape3Slider(p, FGConst::GetParameterID(FGConst::Param_Shape3), nullptr),
    m_shape4Slider(p, FGConst::GetParameterID(FGConst::Param_Shape4), nullptr),
    m_pitch1Slider(p, FGConst::GetParameterID(FGConst::Param_Pitch1), nullptr),
    m_pitch2Slider(p, FGConst::GetParameterID(FGConst::Param_Pitch2), nullptr),
    m_pitch3Slider(p, FGConst::GetParameterID(FGConst::Param_Pitch3), nullptr),
    m_mixSlider(p, FGConst::GetParameterID(FGConst::Param_Mix), nullptr),
    m_fractalDisplay(p),
    m_audioInputVisualiser(p.GetInputWavePlotGenerator(), 50, 390, p.GetCurrentInputMeter()),
    m_audioOutputVisualiser(p.GetOuputWavePlotGenerator(), 50, 390, p.GetCurrentOutputMeter())
{
    setSize (1200, 400);

    addAndMakeVisible(m_inGainSlider);
    addAndMakeVisible(m_outGainSlider);
    addAndMakeVisible(m_delayTimeSlider);
    addAndMakeVisible(m_spreadSlider);
    addAndMakeVisible(m_complexitySlider);
    addAndMakeVisible(m_saturationSlider);
    addAndMakeVisible(m_shape1Slider);
    addAndMakeVisible(m_shape2Slider);
    addAndMakeVisible(m_shape3Slider);
    addAndMakeVisible(m_shape4Slider);
    addAndMakeVisible(m_pitch1Slider);
    addAndMakeVisible(m_pitch2Slider);
    addAndMakeVisible(m_pitch3Slider);
    addAndMakeVisible(m_audioInputVisualiser);
    addAndMakeVisible(m_audioOutputVisualiser);
    addAndMakeVisible(m_fractalInput);
    addAndMakeVisible(m_fractalDisplay);
    addAndMakeVisible(m_mixSlider);
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

    // Set the X coordinates and make mini sliders out of the delay and complexity
    auto delayTimeArea = bottomLeftAreaTemplate;
    delayTimeArea.setX((widthOverThree / 3.0f) + FGConst::GUIElementPadding);
    delayTimeArea.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    delayTimeArea.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_delayTimeSlider.setBounds(delayTimeArea.toNearestInt());

    // We can do the same for the spread area
    auto spreadArea = bottomLeftAreaTemplate;
    spreadArea.setX((widthOverThree / 3.0f) * 2.0f);
    spreadArea.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    spreadArea.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_spreadSlider.setBounds(spreadArea.toNearestInt());

    // We want to create 4 sliders in the bottom area so we set a template for this too and reduce it by the padding element
    auto bottomMiddleAreaTemplate = juce::Rectangle<float>(widthOverThree, heightOverThree * 2.0f, widthOverThree / 4.0f, heightOverThree);
    
    // We can then set the relevant x points for the 4 sliders
    auto shape1Bounds = bottomMiddleAreaTemplate;
    shape1Bounds.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_shape1Slider.setBounds(shape1Bounds.toNearestInt());
    
    auto shape2Bounds = bottomMiddleAreaTemplate;
    shape2Bounds.setX(widthOverThree + (widthOverThree / 4.0f));
    shape2Bounds.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_shape2Slider.setBounds(shape2Bounds.toNearestInt());

    auto shape3Bounds = bottomMiddleAreaTemplate;
    shape3Bounds.setX(widthOverThree + ((widthOverThree / 4.0f) * 2.0f));
    shape3Bounds.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_shape3Slider.setBounds(shape3Bounds.toNearestInt());

    auto shape4Bounds = bottomMiddleAreaTemplate;
    shape4Bounds.setX(widthOverThree + ((widthOverThree / 4.0f) * 3.0f));
    shape4Bounds.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_shape4Slider.setBounds(shape4Bounds.toNearestInt());

    // The bottom right hand slider bank can be set up as the inverse of the left hand slider bank
    auto bottomRightAreaTemplate = juce::Rectangle<float>(widthOverThree * 2, heightOverThree * 2.0f, widthOverThree / 3.0f, heightOverThree);
    
    // We can pad this template area
    //bottomRightAreaTemplate.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);

    auto outGainArea = bottomRightAreaTemplate;
    outGainArea.setX(bottomRightAreaTemplate.getX() + ((widthOverThree / 3.0f) * 2));
    outGainArea.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_outGainSlider.setBounds(outGainArea.toNearestInt());

    auto complexityArea = bottomRightAreaTemplate;
    complexityArea.setX(bottomRightAreaTemplate.getX());
    complexityArea.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    complexityArea.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_complexitySlider.setBounds(complexityArea.toNearestInt());
    
    auto saturationArea = bottomRightAreaTemplate;
    saturationArea.setX(bottomRightAreaTemplate.getX() + (widthOverThree / 3.0f) - FGConst::GUIElementPadding);
    saturationArea.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    saturationArea.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_saturationSlider.setBounds(saturationArea.toNearestInt());

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

    auto pitch1Area = centreRightAreaTemplate;
    pitch1Area.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    pitch1Area.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_pitch1Slider.setBounds(pitch1Area.toNearestInt());

    auto pitch2Area = centreRightAreaTemplate;
    pitch2Area.setX((widthOverThree * 2.0f) + (widthOverThree / 3.0f));
    pitch2Area.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    pitch2Area.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_pitch2Slider.setBounds(pitch2Area.toNearestInt());

    auto pitch3Area = centreRightAreaTemplate;
    pitch3Area.setX((widthOverThree * 2.0f) + (2.0f * (widthOverThree / 3.0f)));
    pitch3Area.reduce(FGConst::MiniSliderReduction, FGConst::MiniSliderReduction);
    pitch3Area.reduce(FGConst::GUIElementPadding, FGConst::GUIElementPadding);
    m_pitch3Slider.setBounds(pitch3Area.toNearestInt());

    
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
