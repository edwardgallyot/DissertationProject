/*
  ==============================================================================

    DelayLineSource.cpp
    Created: 25 Aug 2022 10:23:29pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "DelayLineSource.h"

FGDSP::DelayLineSource::DelayLineSource(LinearDelayLine& delayLine)
    :
    m_delayLine(delayLine),
    m_samplesLeftToProcess(0),
    m_delayTimeSamples(44100.0f)
{
}

void FGDSP::DelayLineSource::SetDelayTimeSamples(float delayTimeSamples)
{
    m_delayTimeSamples = delayTimeSamples;
}

void FGDSP::DelayLineSource::IncrementSamplesProcessedInBlock()
{
    //================================================================================
    // Increment our read index with the pitch
    m_readIndex += m_pitch;
    //================================================================================
    // IMPORTANT:
    // Wrap the index around the buffer and figure out the samples relevant to the size
    // that was written to the block
    // If these change we get into all sorts of trouble reading from delay line
    m_readIndex = fmod(m_readIndex, static_cast<float>(m_delayLine.GetSize()));
}

float FGDSP::DelayLineSource::GetNextSample()
{
    IncrementSamplesProcessedInBlock();
    float sample = m_delayLine.ReadSample(m_readIndex);
    return sample;
}

void FGDSP::DelayLineSource::Init(EdPF::Grains::Source::Essence* essence)
{
    auto tmpEssence = dynamic_cast<DelayLineSource::Essence*>(essence);
    tmpEssence->Configure();
    m_samplesLeftToProcess = m_delayLine.GetBlockSize() - tmpEssence->GetCurrentSampleInBlock();
    m_distanceFromPlayheadScalar = tmpEssence->GetDistanceFromPlayheadScalar();
    m_pitch = tmpEssence->GetGrainPitch();
    m_durationInSamples = tmpEssence->GetGrainDuration();

    m_readIndex = static_cast<float>(m_delayLine.GetWriteIndex()) - (static_cast<float>(m_samplesLeftToProcess));
    m_readIndex -= m_durationInSamples;
    m_readIndex -= (m_delayTimeSamples * m_distanceFromPlayheadScalar);
    m_readIndex += m_delayLine.GetSize();
    
}

void FGDSP::DelayLineSource::Essence::Configure()
{
}
FGDSP::DelayLineSource::Essence::Essence(float duration, float distanceFromPlayHead, int currentSample, float pitch)
{
    distanceFromPlayHeadScalar = distanceFromPlayHead;
    currentSampleInBlock = currentSample;
    grainPitch = pitch;
    grainDuration = duration;
}


