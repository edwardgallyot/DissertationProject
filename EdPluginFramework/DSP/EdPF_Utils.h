/*
  ==============================================================================

    EdPF_Utils.h
    Created: 28 Aug 2022 5:11:15pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_UTILS_H_INCLUDED
#define EDPF_UTILS_H_INCLUDED

namespace EdPF
{
    namespace DSP
    {
        namespace Utils
        {
            static float MsToSamples(float ms, float sampleRate)
            {
                float multiplier = sampleRate / 1000.0f;
                ms *= multiplier;
                return ms;
            }
        }
    }
}

#endif //!EDPF_UTILS_H_INCLUDED