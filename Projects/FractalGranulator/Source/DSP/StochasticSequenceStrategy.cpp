/*
  ==============================================================================

    StochasticSequencyStrategy.cpp
    Created: 25 Aug 2022 10:40:28pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "StochasticSequenceStrategy.h"

int FGDSP::StochasticSequenceStrategy::GetNextInterOnset()
{
    return static_cast<int>(m_random.getSystemRandom().nextFloat() * 1000.0f);
}

float FGDSP::StochasticSequenceStrategy::GetNextDuration()
{
    return 
        EdPF::DSP::Utils::MsToSamples(FGConst::MinimumGrainDuration, static_cast<float>(m_sampleRate))
        + (m_random.getSystemRandom().nextFloat()
        * EdPF::DSP::Utils::MsToSamples(FGConst::MaximumGrainDuration, static_cast<float>(m_sampleRate))
        );
}

float FGDSP::StochasticSequenceStrategy::GetNextDistanceFromPlayheadScalar()
{
    return FGConst::MaxDistanceFromOriginScalar - (m_random.getSystemRandom().nextFloat());
}

float FGDSP::StochasticSequenceStrategy::GetNextPitch()
{
    return std::powf(2.0f, 
        static_cast<float>(
        std::round(
            static_cast<double>(1.0f - 
                (m_random.getSystemRandom().nextFloat()) * FGConst::MaximumPitch)) * 12.0f) 
        / 12.0f
    );
}
