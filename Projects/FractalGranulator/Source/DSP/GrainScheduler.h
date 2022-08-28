/*
  ==============================================================================

    GrainScheduler.h
    Created: 27 Aug 2022 7:06:07pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef GRAIN_SCHEDULER_H_INCLUDED
#define GRAIN_SCHEDULER_H_INCLUDED

#include "../../EdPluginFramework/DSP/EdPF_GrainScheduler.h"
#include "../../EdPluginFramework/DSP/EdPF_GrainPool.h"
#include "../../EdPluginFramework/DSP/EdPF_Utils.h"
#include "StochasticSequenceStrategy.h"
#include "../FractalGranulatorCommon.h"
#include "Grain.h"

namespace FGDSP
{
    class Scheduler : public EdPF::Grains::Scheduler
    {
    public:
        Scheduler(EdPF::Grains::GrainPool<FGDSP::Grain>& pool, std::vector<juce::AudioBuffer<float>>& smoothedValues);
        void SetSampleRate(double sampleRate) { m_sampleRate = sampleRate; }
        float SythesiseNextSample(int i) override;
        float AccumulateGrainOuput(int i);
    private:
        EdPF::Grains::GrainPool<FGDSP::Grain>& m_pooledGrains;
        std::vector<juce::AudioBuffer<float>>& m_smoothedValues;
        double m_sampleRate;
    };
    
}

#endif //!GRAIN_SCHEDULER_H_INCLUDED
