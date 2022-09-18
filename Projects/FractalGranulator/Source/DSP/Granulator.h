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
#include "../../EdPluginFramework/DSP/EdPF_GrainPool.h"
#include "GrainScheduler.h"
#include "StochasticSequenceStrategy.h"
#include "../../EdPluginFramework/Utils/EdPF_Fifo.h"
#include "../../EdPluginFramework/DSP/EdPF_Granulator.h"


namespace FGDSP
{
    class Granulator : public EdPF::Granulator
    {
    public:
        Granulator(EdPF::AudioProcessor& p, LinearDelayLine& delayLine, std::vector<juce::AudioBuffer<float>>& smoothedValue);
        ~Granulator() override;

        void PrepareToPlay(double sampleRate, int numSamplesExpected) override;

        void ProcessSamples(juce::AudioBuffer<float>& buffer) override;

        EdPF::Fifo<GrainPlotData>* GetSchedulerFifo() { return m_scheduler.GetFifo(); }
        void RegisterFifoReader(juce::Component* reader) { m_scheduler.RegisterFifoReader(reader); }
        void DeregisterFifoReader() { m_scheduler.DeregisterFifoReader(); }
    private:
        EdPF::AudioProcessor& m_processor;
        Scheduler m_scheduler;
        EdPF::Grains::GrainPool<Grain> m_grainPool;
    };
}

#endif //!GRANULATOR_H_INCLUDED