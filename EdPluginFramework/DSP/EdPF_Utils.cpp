/*
  ==============================================================================

    EdPF_Utils.cpp
    Created: 28 Aug 2022 5:11:15pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "EdPF_Utils.h"

float EdPF::DSP::Utils::MsToSamples(float ms, float sampleRate)
{
    float multiplier = sampleRate / 1000.0f;
    ms *= multiplier;
    return ms;
}

float EdPF::DSP::Utils::SamplesToMs(float samples, float sampleRate)
{
    return samples * (1000.0f / sampleRate);
}


