/*
  ==============================================================================

    Granulator.cpp
    Created: 25 Aug 2022 10:39:57pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "Granulator.h"

FGDSP::Granulator::Granulator(LinearDelayLine& delayLine, std::vector<juce::AudioBuffer<float>>& smoothedValues, juce::AudioPlayHead::CurrentPositionInfo& currentPositionInfo) :
    m_scheduler(m_grainPool, smoothedValues, currentPositionInfo),
    m_grainPool(FGConst::MaxGrains)
{
    // Set the sequence strategy for out scheduler
    m_scheduler.SetSequenceStrategy(std::make_unique<FractalSequenceStrategy>());
    // Set the Envelope type and the source type for each of our grains
    for (auto& grain : m_grainPool.GetPool())
    {
        grain.SetEnvelope(std::make_unique<ParabolicEnvelope>());
        grain.SetSource(std::make_unique <DelayLineSource>(delayLine));
    }
}

FGDSP::Granulator::~Granulator()
{
}

void FGDSP::Granulator::PrepareToPlay(double sampleRate, int /*numSamplesExpected*/)
{  
    m_scheduler.SetSampleRate(sampleRate);
}

void FGDSP::Granulator::ProcessSamples(juce::AudioBuffer<float>& buffer)
{
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        float sample = m_scheduler.SythesiseNextSample(i);
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            buffer.setSample(channel, i, sample);
        }
    }
}
