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

float EdPF::DSP::Utils::SamplesToMs(const float samples, const float sampleRate)
{
    return samples * (1000.0f / sampleRate);
}

float EdPF::DSP::Utils::GetSpeedFromPitch(const float noteValue)
{
    return pow(2.0f, noteValue / 12.0f);
}

float EdPF::DSP::Utils::GetPitchFromSpeed(const float speed)
{
    return 12.0f * log2f(speed);
}
