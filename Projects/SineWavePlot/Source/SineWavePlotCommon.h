/*
  ==============================================================================

    SineWavePlotCommon.h
    Created: 29 Jul 2022 7:13:17am
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef SINE_WAVE_PLOT_COMMON_H
#define SINE_WAVE_PLOT_COMMON_H
#include <JuceHeader.h>
#include <vector>

namespace SWP
{

    const enum Params
    {
        Param_Freq,
        Param_Amp,
        NumOfParams
    };


    const String ParamIds[NumOfParams] =
    {
        "Frequency",
        "Amplitude"
    };

}

#endif // !SINE_WAVE_PLOT_COMMON_H
