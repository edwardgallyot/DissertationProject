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
    m_serpinskiBounds(0),
    m_complexityScalar(0.0f)
{
    m_serpinskiBounds.push_back({FGConst::MinDistanceFromOriginScalar, FGConst::MinimumPitch});
    m_serpinskiBounds.push_back({ FGConst::MaxDistanceFromOriginScalar - (FGConst::MinDistanceFromOriginScalar / 2.0f), FGConst::MaximumPitch });
    m_serpinskiBounds.push_back({FGConst::MaxDistanceFromOriginScalar, FGConst::MinimumPitch});

    m_currentPoint = m_serpinskiBounds[0];
}

int FGDSP::FractalSequenceStrategy::GetNextInterOnset()
{
    return static_cast<int>(FGConst::MinimumInteronsetSamples) + static_cast<int>(m_random.getSystemRandom().nextFloat() * m_complexityScalar * (FGConst::MaximumInteronset - FGConst::MaximumInteronset));
}

float FGDSP::FractalSequenceStrategy::GetNextDuration()
{
    return
        EdPF::DSP::Utils::MsToSamples(FGConst::MinimumGrainDuration, static_cast<float>(m_sampleRate))
        + (m_complexityScalar 
            *(m_random.getSystemRandom().nextFloat()
            * EdPF::DSP::Utils::MsToSamples(FGConst::MaximumGrainDuration - FGConst::MinimumGrainDuration, static_cast<float>(m_sampleRate))
            )
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

    juce::AffineTransform transform;

    // Move Point to the centre of the origin
    m_currentPoint = m_currentPoint - juce::Point<float>(0.5f, 0.75f);
    m_currentPoint = m_currentPoint - juce::Point<float>(FGConst::MinDistanceFromOriginScalar, FGConst::MinimumPitch);

    // Apply the transformations
    m_currentPoint.applyTransform
    (

        transform.scale
        (
            0.01f + (m_shape1 * 0.99f),
            0.01f + (m_shape2 * 0.99f)
        ).rotated
        (juce::MathConstants<float>::twoPi * m_shape3)
        .sheared
        (-m_shape4 * 2.0f, m_shape4 * 2.0f)
    );

    // Move them back from whence they came...
    m_currentPoint = m_currentPoint + juce::Point<float>(0.5f, 0.75f);
    m_currentPoint = m_currentPoint + juce::Point<float>(FGConst::MinDistanceFromOriginScalar, FGConst::MinimumPitch);

    // CHECK X BOUNDS
    if (m_currentPoint.getX() < FGConst::MinDistanceFromOriginScalar)
    {
        m_currentPoint.setX(FGConst::MinDistanceFromOriginScalar);
    }
    if (m_currentPoint.getX() > FGConst::MaxDistanceFromOriginScalar)
    {
        m_currentPoint.setX(FGConst::MaxDistanceFromOriginScalar);
    }


    // CHECK Y BOUNDS
    if (m_currentPoint.getY() < FGConst::MinimumPitch)
    {
        m_currentPoint.setY(FGConst::MinimumPitch);
    }
    if (m_currentPoint.getY() > FGConst::MaximumPitch)
    {
        m_currentPoint.setY(FGConst::MaximumPitch);
    }
}

