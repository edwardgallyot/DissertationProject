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
        Param_Spread,
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
    const float DelayTimeMin = 100.0f;

    const float MiniSliderReduction = 15.0f;

    const float GUIElementPadding = 5.0f;

    const int MaxGrains = 1000;
}

#endif // !FRACTAL_GRANULATOR_COMMON_H

