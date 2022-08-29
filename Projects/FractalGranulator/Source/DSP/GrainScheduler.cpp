/*
  ==============================================================================

    GrainScheduler.cpp
    Created: 27 Aug 2022 7:06:07pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "GrainScheduler.h"

FGDSP::Scheduler::Scheduler(EdPF::Grains::GrainPool<FGDSP::Grain>& pool, std::vector<juce::AudioBuffer<float>>& smoothedValues) :
    m_pooledGrains(pool),
    m_smoothedValues(smoothedValues),
    m_sampleRate(0.0)
{
}

float FGDSP::Scheduler::SythesiseNextSample(int i)
{
    EdPF::Grains::Scheduler::IncrementSample();
    // Call this if we can get a grain from the pool. It allows us to access our custom sequence strategy
    auto* strategyPtr = dynamic_cast<StochasticSequenceStrategy*>(GetUnderlyingSequenceStrategy());
    // We should activate a grain if the Scheduler tells us to
    if (ShouldActivateGrain())
    {
        Grain* grain = m_pooledGrains.FindNextFreeGrain();
        if (grain != nullptr)
        {
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

            // Activate the grain with the source essence or how it's gonna read from the delay line
            DelayLineSource::Essence tmpSrcEssence(nextDur / nextPitch, strategyPtr->GetNextDistanceFromPlayheadScalar(), i, nextPitch);

            // Also give it an envelope relevant to the duration
            ParabolicEnvelope::Essence tmpEnvEssence(nextDur / nextPitch, 0.5f);
            
            // Activate a grain with it's durations and it's essences
            grain->Activate(static_cast<int>(nextDur / nextPitch), &tmpEnvEssence, &tmpSrcEssence);
        }
    }

    // Here we accumulate the grain output
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
