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
    // We should activate a grain if the Scheduler tells us to
    if (ShouldActivateGrain())
    {
        Grain* grain = m_pooledGrains.FindNextFreeGrain();
        if (grain != nullptr)
        {
            // Call this if we can get a grain from the pool.
            auto strategyPtr = dynamic_cast<StochasticSequenceStrategy*>(GetUnderlyingSequenceStrategy());
            // We will cache the next duration so we don't call it multiple times
            float nextDur = GetNextDuration();
            // Activate the grain with the source essence or how it's gonna read from the delay line
            DelayLineSource::Essence tmpSrcEssence
            (
                nextDur, 
                strategyPtr->GetNextDistanceFromPlayheadScalar(),
                i
            );
            // Also give it an envelope relevant to the duration
            ParabolicEnvelope::Essence tmpEnvEssence(nextDur, 0.5f);
            grain->Activate(nextDur, &tmpEnvEssence, &tmpSrcEssence);
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
            dynamic_cast<DelayLineSource*>(grain.GetUnderlyingSource())->SetDelayTimeSamples
            (  
                EdPF::DSP::Utils::MsToSamples
                (
                        m_smoothedValues[FGConst::Params::Param_DelayTime].getSample(0, i), 
                        static_cast<float>(m_sampleRate)
                )
            );
            // Process the sample for the grain
            grainOutput += grain.ProcessSample();
        }
    }
    return grainOutput;
}
