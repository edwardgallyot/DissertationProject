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
    return 1000.0f + (m_random.getSystemRandom().nextFloat() * 10000.0f);   
}

float FGDSP::StochasticSequenceStrategy::GetNextDistanceFromPlayheadScalar()
{
    return 1.0f + m_random.getSystemRandom().nextFloat();
}

float FGDSP::StochasticSequenceStrategy::GetNextPitch()
{
    return std::powf(2.0f, static_cast<float>(std::round(static_cast<double>(1.0f - (m_random.getSystemRandom().nextFloat()) * 2.0f)) * 12.0f) / 12.0f);
}
