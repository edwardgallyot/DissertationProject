/*
  ==============================================================================

    Grain.cpp
    Created: 27 Aug 2022 8:31:15pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "Grain.h"

FGDSP::Grain::Grain()
{
}

FGDSP::Grain::~Grain()
{
}

void FGDSP::Grain::Activate(int duration, EdPF::Grains::Envelope::Essence* envEssence, EdPF::Grains::Source::Essence* srcEssence)
{
    ResetPhase();
    SetDuration(duration);
    InitialiseSource(srcEssence);
    InitialiseEnvelope(envEssence);
    SetIsActive(true);
}


void FGDSP::Grain::Deactivate()
{
    SetIsActive(false);
    ResetPhase();
}


float FGDSP::Grain::ProcessSample()
{
    if (GetPhase() >= GetDuration())
    {
        Deactivate();
    }
    else
    {
        IncrementPhase(1);
    }

    float nextSourceSample = GetNextSourceSample();
    float nextEnvelopeSample = GetNextEnvelopeSample();

    return nextEnvelopeSample * nextSourceSample;
}
