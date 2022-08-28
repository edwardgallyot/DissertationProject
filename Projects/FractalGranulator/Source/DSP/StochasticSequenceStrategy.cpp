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
    return 4410.0f + (m_random.getSystemRandom().nextFloat() * 44100.0f);   
}

float FGDSP::StochasticSequenceStrategy::GetNextDistanceFromPlayheadScalar()
{
    return (m_random.getSystemRandom().nextFloat() * 2.0f) - 1.0f;
}

float FGDSP::StochasticSequenceStrategy::GetNextPitch()
{
    return m_random.getSystemRandom().nextFloat() + 0.5f;
}
