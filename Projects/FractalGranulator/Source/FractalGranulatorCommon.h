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
        Param_Mix,
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
        Param_PitchQuantize,
        Param_PitchShift,
        Param_Harmony,
        NumOfParams
    };


    const juce::String ParamIds[NumOfParams] =
    {
        "Mix",
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
        "PitchQuantize",
        "PitchShift",
        "PitchHarmony"
    };

    juce::StringRef GetParameterID(Params param);

    const float DelayTimeMax = 2000.0f;
    const float DelayTimeMin = 5.0f;

    const float MiniSliderReduction = 15.0f;

    const float GUIElementPadding = 5.0f;

    const int MaxGrains = 500;

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

    const juce::Colour BackgroundColour = juce::Colour(109, 56, 46);
    const juce::Colour GrainColour = juce::Colour(232, 118, 125);
    const juce::Colour WaveFormColour = juce::Colour(220, 218, 99);

    const enum Harmony
    {
        First,
        Octave,
        PowerChord,
        Major,
        Minor,
        Major7,
        Minor7,
        Major9,
        Minor9,
        NumKeys
    };

    const std::vector<float> FirstHarmony{ 0.0f };
    const std::vector<float> OctaveHarmony{ -12.0f, 0.0f, 12.0f };
    const std::vector<float> PowerChordHarmony{ -12.0f, - 5.0f, 0.0f, 7.0f, 12.0f };
    const std::vector<float> MajorHarmony{ -12.0f, -5.0f, 0.0f, 4.0f, 7.0f, 12.0f };
    const std::vector<float> MinorHarmony{ -12.0f, -5.0f, 0.0f, 3.0f, 7.0f, 12.0f };
    const std::vector<float> Major7Harmony{ -12.0f, -5.0f, 0.0f, 4.0f, 7.0f, 11.0f };
    const std::vector<float> Minor7Harmony{ -12.0f, -5.0f, 0.0f, 3.0f, 7.0f, 11.0f };
    const std::vector<float> Major9Harmony{ -12.0f, -5.0f, -1.0f, 2.0f, 4.0f, 7.0f, 12.0f };
    const std::vector<float> Minor9Harmony{ -12.0f, -5.0f, -1.0f, 2.0f, 3.0f, 7.0f, 12.0f };
   

    const std::array<const std::vector<float>*, NumKeys> Harmonies
	{
        &FirstHarmony,
        &OctaveHarmony,
        &PowerChordHarmony,
        &MajorHarmony,
        &MinorHarmony,
        &Major7Harmony,
        &Minor7Harmony,
        &Major9Harmony,
        &Minor9Harmony,
	};
}

#endif // !FRACTAL_GRANULATOR_COMMON_H

