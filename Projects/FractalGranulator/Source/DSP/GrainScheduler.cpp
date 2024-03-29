/*
  ==============================================================================

    GrainScheduler.cpp
    Created: 27 Aug 2022 7:06:07pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "GrainScheduler.h"

FGDSP::Scheduler::Scheduler(EdPF::AudioProcessor& p, EdPF::Grains::GrainPool<FGDSP::Grain>& pool, std::vector<juce::AudioBuffer<float>>& smoothedValues) :
    m_processor(p),
    m_pooledGrains(pool),
    m_smoothedValues(smoothedValues),
    m_sampleRate(0.0),
    m_fifo(1000),
    m_totalNumberWritten(0),
    m_fifoReader(nullptr),
    m_newFifoData()
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

    // We want to set the complexity scalar from the smoothed params
    float complexityScalar = m_smoothedValues[FGConst::Params::Param_Complexity].getSample(0, i);
    strategyPtr->SetComplexityScalar(complexityScalar);

    // We also want to set the shape controls for our sequence strategy
    strategyPtr->SetShapeOne(m_smoothedValues[FGConst::Param_Shape1].getSample(0, i));
    strategyPtr->SetShapeTwo(m_smoothedValues[FGConst::Param_Shape2].getSample(0, i));
    strategyPtr->SetShapeThree(m_smoothedValues[FGConst::Param_Shape3].getSample(0, i));
    strategyPtr->SetShapeFour(m_smoothedValues[FGConst::Param_Shape4].getSample(0, i));

    // We should activate a grain if the Scheduler tells us to
    if (ShouldActivateGrain())
    {
        Grain* grain = m_pooledGrains.FindNextFreeGrain();
        if (grain != nullptr)
        {
            // With a fractal sequency strategy it will all come down to the chaos game
            strategyPtr->DoChaosGame();
            
            if (strategyPtr->CheckPointsBounds())
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
                
                float distanceFromOriginScalar = strategyPtr->GetNextDistanceFromPlayheadScalar();

                if (static_cast<bool>(m_processor.GetParameterAsValue(FGConst::GetParameterID(FGConst::Param_PitchQuantize)).getValue()))
                {
                    m_pitchQuantizer.SetHarmony(
                        static_cast<FGConst::Harmony>(
                        static_cast<int>(
                        m_processor.GetParameterAsValue(
                        GetParameterID(FGConst::Params::Param_Harmony))
                        .getValue())));


                    
                    auto nextRealPitch = EdPF::DSP::Utils::GetPitchFromSpeed(nextPitch);

                	nextRealPitch = m_pitchQuantizer.GetQuantizedPitch(nextRealPitch);
                    auto pitchShift = static_cast<float>(m_processor.GetParameterAsValue(FGConst::GetParameterID(FGConst::Param_PitchShift)).getValue());
                    
                    nextPitch = EdPF::DSP::Utils::GetSpeedFromPitch(nextRealPitch + pitchShift);
                }

                // We will do a pitch check here to make sure that we're still on the 
                auto pitchCheckValue = EdPF::DSP::Utils::GetPitchFromSpeed(nextPitch);
                if (pitchCheckValue >= -12.0f && pitchCheckValue <= 12.0f)
                {
                    // Scale the duration so we only make the calculation once
                    float scaledDur = nextDur / nextPitch;

                    // Activate the grain with the source essence or how it's gonna read from the delay line
                    DelayLineSource::Essence tmpSrcEssence(scaledDur, distanceFromOriginScalar, i, nextPitch);

                    // Also give it an envelope relevant to the duration
                    ParabolicEnvelope::Essence tmpEnvEssence(scaledDur, 0.5f);
                    // Activate a grain with it's durations and it's essences
                    grain->Activate(static_cast<int>(scaledDur), &tmpEnvEssence, &tmpSrcEssence);

                    // Then configure the FIFO data
                    float nextDurMs = EdPF::DSP::Utils::SamplesToMs(nextDur, static_cast<float>(m_sampleRate));
                    m_newFifoData.Configure(nextDurMs, nextPitch, distanceFromOriginScalar);
                    // If we don't have a reader we won't need to plot any grain data
                    if (m_fifoReader != nullptr)
                    {
                        // Use assignment for debugging.
                        /*int x =*/ m_fifo.AddToFifo(&m_newFifoData, 1);
                    }
                }
                
            }

        }
    }
    // Here we accumulate the grain output and this is what will passed up to the granulator
    return AccumulateGrainOuput(i);
}


float FGDSP::Scheduler::AccumulateGrainOuput(int /*i*/)
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
