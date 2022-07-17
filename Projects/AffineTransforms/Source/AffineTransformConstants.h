/*
  ==============================================================================

    AffineTransformConstants.h
    Created: 16 Jul 2022 7:49:00pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef AFFINE_TRANSFORM_CONSTANTS_H
#define AFFINE_TRANSFORM_CONSTANTS_H


namespace AffineTransformConstants
{
    const juce::String valueTreeID = "TREE";

    const int sliderBankHeight = 100;

    enum ParameterEnum
    {
        slider1,
        slider2,
        slider3,
        slider4,
        slider5,
        slider6,
        NumParams
    };

    const std::array<juce::String, NumParams> ParameterIDS
    {
        "Slider1",
        "Slider2",
        "Slider3",
        "Slider4",
        "Slider5",
        "Slider6"
    };

    const std::array<const juce::NormalisableRange<double>, NumParams> ParamRanges
    {
        juce::NormalisableRange<double>(-1.0f, 1.0f),
        juce::NormalisableRange<double>(-1.0f, 1.0f),
        juce::NormalisableRange<double>(-1.0f, 1.0f),
        juce::NormalisableRange<double>(-1.0f, 1.0f),
        juce::NormalisableRange<double>(-1.0f, 1.0f),
        juce::NormalisableRange<double>(-1.0f, 1.0f)
    };

}

#endif // !AFFINE_TRANSFORM_CONSTANTS_H
