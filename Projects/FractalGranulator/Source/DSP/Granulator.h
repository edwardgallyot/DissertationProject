/*
  ==============================================================================

    Granulator.h
    Created: 25 Aug 2022 10:39:57pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef GRANULATOR_H_INCLUDED
#define GRANULATOR_H_INCLUDED

#include <JuceHeader.h>
#include "../../EdPluginFramework/DSP/EdPF_DelayLine.h"
#include "../DSP/DelayLineSource.h"
#include "../../EdPluginFramework/DSP/EdPF_GrainPool.h"
#include "../../EdPluginFramework/DSP/EdPF_Grain.h"
#include "../DSP/ParabolicEnvelope.h"
#include "GrainScheduler.h"
#include "StochasticSequenceStrategy.h"
#include "../../EdPluginFramework/Utils/EdPF_Fifo.h"


namespace FGDSP
{
    class Granulator
    {
    public:
        Granulator(LinearDelayLine& delayLine, std::vector<juce::AudioBuffer<float>>& smoothedValues, juce::AudioPlayHead::CurrentPositionInfo&);
        ~Granulator();

        void PrepareToPlay(double sampleRate, int numSamplesExpected);

        void ProcessSamples(juce::AudioBuffer<float>& buffer);

        EdPF::Fifo<GrainPlotData>* GetSchedulerFifo() { return m_scheduler.GetFifo(); }
        void RegisterFifoReader(juce::Component* reader) { m_scheduler.RegisterFifoReader(reader); }
        void DeregisterFifoReader() { m_scheduler.DeregisterFifoReader(); }

    private:
        Scheduler m_scheduler;
        EdPF::Grains::GrainPool<Grain> m_grainPool;
    };
}

#endif //!GRANULATOR_H_INCLUDED