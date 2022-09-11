/*
  ==============================================================================

    EdPF_Utils.h
    Created: 28 Aug 2022 5:11:15pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_UTILS_H_INCLUDED
#define EDPF_UTILS_H_INCLUDED

#include <cmath>

namespace EdPF
{
    namespace DSP
    {
        namespace Utils
        {
            float MsToSamples(float ms, float sampleRate);
            float SamplesToMs(float ms, float sampleRate);
        	float GetSpeedFromPitch(float noteValue);
            float GetPitchFromSpeed(float speed);
        }
    }
}


#endif //!EDPF_UTILS_H_INCLUDED