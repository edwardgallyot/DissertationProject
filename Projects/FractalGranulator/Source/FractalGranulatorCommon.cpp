/*
  ==============================================================================

    FractalGranulatorCommon.cpp
    Created: 7 Aug 2022 4:52:50pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "FractalGranulatorCommon.h"

juce::StringRef FGConst::GetParameterID(Params param)
{
    return FGConst::ParamIds[param];
}
