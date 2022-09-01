/*
  ==============================================================================

    FractalGranulatorCommon.h
    Created: 3 Aug 2022 2:46:37pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef FRACTAL_GRANULATOR_COMMON_H
#define FRACTAL_GRANULATOR_COMMON_H

#include "JuceHeader.h"

namespace FGConst
{
    const enum Params
    {
        Param_InGain,
        Param_OutGain,
        Param_DelayTime,
        Param_Feedback,
        Param_Complexity,
        Param_Saturation,
        Param_Shape1,
        Param_Shape2,
        Param_Shape3,
        Param_Shape4,
        Param_Pitch1,
        Param_Pitch2,
        Param_Pitch3,
        NumOfParams
    };


    const juce::String ParamIds[NumOfParams] =
    {
        "InGain",
        "OutGain",
        "DelayTime",
        "Spread",
        "Complexity",
        "Saturation",
        "Shape1",
        "Shape2",
        "Shape3",
        "Shape4",
        "Pitch1",
        "Pitch2",
        "Pitch3"
    };

    juce::StringRef GetParameterID(Params param);

    const float DelayTimeMax = 4000.0f;
    const float DelayTimeMin = 5.0f;

    const float MiniSliderReduction = 15.0f;

    const float GUIElementPadding = 5.0f;

    const int MaxGrains = 1000;

    const int NumGrainPlotPoints = MaxGrains;

    const int GUITimerHz = 50;

    const float MinimumGrainDuration = 200.0f;
    const float MaximumGrainDuration = 2000.0f;

    const float MinimumPitch = 0.5f;
    const float MaximumPitch = 2.0f;

    const float MinDistanceFromOriginScalar = 1.0f;
    const float MaxDistanceFromOriginScalar = 2.0f;

    const float MinimumInteronsetSamples = 150.0f;
    const float MaximumInteronset = 4000.0f;
}

#endif // !FRACTAL_GRANULATOR_COMMON_H

