/*
  ==============================================================================

    Grain.h
    Created: 27 Aug 2022 8:31:15pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef GRAIN_H_INLCUDED
#define GRAIN_H_INCLUDED

#include "../../EdPluginFramework/DSP/EdPF_Grain.h"
#include "../DSP/ParabolicEnvelope.h"
#include "../DSP/DelayLineSource.h"
#include "../../EdPluginFramework/DSP/EdPF_GrainEnvelope.h"

namespace FGDSP
{
    class Grain : public EdPF::Grains::Grain
    {
    public:
        Grain();
        ~Grain();
        void Activate(int duration, EdPF::Grains::Envelope::Essence* envEssence, EdPF::Grains::Source::Essence* srcEssence) override;
        void Deactivate() override;
        float ProcessSample() override;
    private:
    };
}

#endif GRAIN_H_INCLUDED
