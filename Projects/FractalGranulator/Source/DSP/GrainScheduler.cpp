/*
  ==============================================================================

    GrainScheduler.cpp
    Created: 27 Aug 2022 7:06:07pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "GrainScheduler.h"

FGDSP::Scheduler::Scheduler(EdPF::Grains::GrainPool<FGDSP::Grain>& pool, std::vector<juce::AudioBuffer<float>>& smoothedValues, juce::AudioPlayHead::CurrentPositionInfo& currentPositionInfo) :
    m_pooledGrains(pool),
    m_smoothedValues(smoothedValues),
    m_sampleRate(0.0),
    m_fifo(1000),
    m_totalNumberWritten(0),
    m_fifoReader(nullptr),
    m_newFifoData(),
    m_currentPositionInfo(currentPositionInfo)
{
}

void FGDSP::Scheduler::SetSampleRate(double sampleRate)
{
    m_sampleRate = sampleRate;
    dynamic_cast<FractalSequenceStrategy*>(GetUnderlyingSequenceStrategy())->SetSampleRate(sampleRate);
}

float FGDSP::Scheduler::SythesiseNextSample(int i)
{
    EdPF::Grains::Scheduler::IncrementSample();
    // Call this if we can get a grain from the pool. It allows us to access our custom sequence strategy
    auto* strategyPtr = dynamic_cast<FractalSequenceStrategy*>(GetUnderlyingSequenceStrategy());
    // We should activate a grain if the Scheduler tells us to
    if (ShouldActivateGrain() /*&& m_currentPositionInfo.isPlaying*/)
    {
        Grain* grain = m_pooledGrains.FindNextFreeGrain();
        if (grain != nullptr)
        {
            // With a fractal sequency strategy it will all come down to the chaos game
            strategyPtr->DoChaosGame();

            dynamic_cast<DelayLineSource*>(grain->GetUnderlyingSource())->SetDelayTimeSamples
            (
                EdPF::DSP::Utils::MsToSamples
                (
                    m_smoothedValues[FGConst::Params::Param_DelayTime].getSample(0, i),
                    static_cast<float>(m_sampleRate)
                )
            );

            // We will cache the next duration so we don't call it multiple times
            float nextDur = GetNextDuration();

            // We will also cache the next pitch
            float nextPitch = strategyPtr->GetNextPitch();

            // Scale the duration so we only make the calculation once
            float scaledDur = nextDur / nextPitch;

            float distanceFromOriginScalar = strategyPtr->GetNextDistanceFromPlayheadScalar();

            // Activate the grain with the source essence or how it's gonna read from the delay line
            DelayLineSource::Essence tmpSrcEssence(scaledDur, distanceFromOriginScalar, i, nextPitch);

            // Also give it an envelope relevant to the duration
            ParabolicEnvelope::Essence tmpEnvEssence(scaledDur, 0.5f);
            
            // Activate a grain with it's durations and it's essences
            grain->Activate(static_cast<int>(scaledDur), &tmpEnvEssence, &tmpSrcEssence);

            float nextDurMs = EdPF::DSP::Utils::SamplesToMs(nextDur, static_cast<float>(m_sampleRate));
          
            m_newFifoData.Configure(nextDurMs, nextPitch, distanceFromOriginScalar);
                

            // If we don't have a reader we won't need to plot any grain data
            if (m_fifoReader != nullptr)
            {
                int x = m_fifo.AddToFifo(&m_newFifoData, 1);
            }
        }
    }
    // Here we accumulate the grain output and this is what will passed up to the granulator
    return AccumulateGrainOuput(i);
}


float FGDSP::Scheduler::AccumulateGrainOuput(int i)
{
    float grainOutput{ 0.0f };
    for (auto& grain : m_pooledGrains.GetPool())
    {
        if (grain.GetIsActive())
        {
            
            // Process the sample for the grain
            grainOutput += grain.ProcessSample();
        }
    }
    return grainOutput;
}
