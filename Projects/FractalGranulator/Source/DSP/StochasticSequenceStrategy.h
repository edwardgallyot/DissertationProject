/*
  ==============================================================================

    StochasticSequencyStrategy.h
    Created: 25 Aug 2022 10:40:27pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef STOCHASTIC_SEUQUENCE_STRATEGY_H_INCLUDED
#define STOCHASTIC_SEUQUENCE_STRATEGY_H_INCLUDED

#include "../../EdPluginFramework/DSP/EdPF_SequenceStrategy.h"
#include "JuceHeader.h"

namespace FGDSP
{
    class StochasticSequenceStrategy : public EdPF::Grains::SequenceStrategy
    {
    public:
        int GetNextInterOnset() override;
        float GetNextDuration() override;
        float GetNextDistanceFromPlayheadScalar();
        float GetNextPitch();
    private:
        juce::Random m_random;
    };
}

#endif //!STOCHASTIC_SEUQUENCE_STRATEGY_H_INCLUDED
