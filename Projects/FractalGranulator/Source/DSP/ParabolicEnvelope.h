/*
  ==============================================================================

    ParabolicEnvelope.h
    Created: 25 Aug 2022 10:24:59pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef PARABOLIC_ENVELOPE_H_INCLUDED
#define PARABOLIC_ENVELOPE_H_INCLUDED

#include "JuceHeader.h"
#include "../FractalGranulatorCommon.h"
#include "../../EdPluginFramework/DSP/EdPF_GrainEnvelope.h"

namespace FGDSP
{
    class ParabolicEnvelope : public EdPF::Grains::Envelope
    {
    public:
        ParabolicEnvelope();
        ~ParabolicEnvelope();
        float GetNextSample() override;

        struct Essence : public EdPF::Grains::Envelope::Essence
        {
            Essence(float duration, float amplitude);
            // Don't call this until you've set the duration and amplitude!
            void Configure() override;
            float GetInitialAmplitude() { return 0.0f; }
            float GetSlope() { return slope; }
            float GetCurve() { return curve; }
        private:
            // Things to be configured within configure....
            float duration;
            float amplitude;
            float rdur{ 0.0f };
            float rdur2{ 0.0f };
            float curve{ 0.0f };
            float slope{ 0.0f };
        };

        // He're we don't use a unique_ptr because we don't care about the lifetime of the Essence
        // we use it in the init function and pass it away
        void Init(EdPF::Grains::Envelope::Essence* essence) override;

    private:
        float m_currentAmplitude;
        float m_slope;
        float m_curve;
    };
}



#endif //!PARABOLIC_ENVELOPE_H_INCLUDED
