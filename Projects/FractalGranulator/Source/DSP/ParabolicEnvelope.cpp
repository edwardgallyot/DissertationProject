/*
  ==============================================================================

    ParabolicEnvelope.cpp
    Created: 25 Aug 2022 10:24:59pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "ParabolicEnvelope.h"

FGDSP::ParabolicEnvelope::ParabolicEnvelope()
{
}

FGDSP::ParabolicEnvelope::~ParabolicEnvelope()
{
}

float FGDSP::ParabolicEnvelope::GetNextSample()
{
    m_currentAmplitude += m_slope;
    m_slope += m_curve;
    //DBG(m_currentAmplitude);
    if (m_currentAmplitude < 0.0f)
    {
        // Sometimes the parabolic envelope can trail a bit beyond the phase of the grain and end up dipping off pretty quick
        // In this instance we probably wanna give 0 back to the grain so that it doesn't clip...
        return 0.0f;
    }
    return m_currentAmplitude;
}

void FGDSP::ParabolicEnvelope::Init(EdPF::Grains::Envelope::Essence* essence)
{
    // The Envelope has to be initalised with an essence this makes sure that only the correct parameters are used.
    auto tmpEssence = dynamic_cast<FGDSP::ParabolicEnvelope::Essence*>(essence);
    m_currentAmplitude = tmpEssence->GetInitialAmplitude();
    m_curve = tmpEssence->GetCurve();
    m_slope = tmpEssence->GetSlope();
}

FGDSP::ParabolicEnvelope::Essence::Essence(float dur, float grainAmp)
{
    amplitude = grainAmp;
    duration = dur;
    Configure();
}

void FGDSP::ParabolicEnvelope::Essence::Configure()
{
    rdur = 1.0f / duration;
    rdur2 = rdur * rdur;
    slope = 4.0f * amplitude * (rdur - rdur2);
    curve = -8.0f * amplitude * rdur2;
}
