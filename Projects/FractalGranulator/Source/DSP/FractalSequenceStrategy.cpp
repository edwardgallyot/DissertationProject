/*
  ==============================================================================

    FractalSequenceStrategy.cpp
    Created: 29 Aug 2022 6:20:42pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "FractalSequenceStrategy.h"

FGDSP::FractalSequenceStrategy::FractalSequenceStrategy()
    :
    m_serpinskiBounds(0)
{
    m_serpinskiBounds.push_back({FGConst::MinDistanceFromOriginScalar, FGConst::MinimumPitch});
    m_serpinskiBounds.push_back({ 1.5f, FGConst::MaximumPitch });
    m_serpinskiBounds.push_back({FGConst::MaxDistanceFromOriginScalar, FGConst::MinimumPitch});

    m_currentPoint = m_serpinskiBounds[0];
}

int FGDSP::FractalSequenceStrategy::GetNextInterOnset()
{
    return 100.0f + static_cast<int>(m_random.getSystemRandom().nextFloat() * 500.0f);
}

float FGDSP::FractalSequenceStrategy::GetNextDuration()
{
    return
        EdPF::DSP::Utils::MsToSamples(FGConst::MinimumGrainDuration, static_cast<float>(m_sampleRate))
        + (m_random.getSystemRandom().nextFloat()
            * EdPF::DSP::Utils::MsToSamples(FGConst::MaximumGrainDuration, static_cast<float>(m_sampleRate))
            );
}

float FGDSP::FractalSequenceStrategy::GetNextDistanceFromPlayheadScalar()
{
    return m_currentPoint.getX();
}

float FGDSP::FractalSequenceStrategy::GetNextPitch()
{
    return m_currentPoint.getY();
}

void FGDSP::FractalSequenceStrategy::DoChaosGame()
{
    int randomInt = m_random.nextInt({ 0, 3 });
    m_currentPoint = 0.5f * (m_currentPoint + m_serpinskiBounds[randomInt]);
}

