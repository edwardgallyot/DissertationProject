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
#include "../../EdPluginFramework/Utils/EdPF_Fifo.h"
#include "../GUI/GrainPlotData.h"
#include "StochasticSequenceStrategy.h"
#include "FractalSequenceStrategy.h"
#include "../FractalGranulatorCommon.h"
#include "Grain.h"
#include <JuceHeader.h>

namespace FGDSP
{
    class Scheduler : public EdPF::Grains::Scheduler
    {
    public:
        Scheduler(EdPF::Grains::GrainPool<FGDSP::Grain>& pool, std::vector<juce::AudioBuffer<float>>& smoothedValues, juce::AudioPlayHead::CurrentPositionInfo&);
        // Set the sample rate so we can make some conversions to time
        void SetSampleRate(double sampleRate);
        // This is what the granulator will call
        float SythesiseNextSample(int i) override;

        void RegisterFifoReader(juce::Component* reader) { m_fifoReader = reader; }
        void DeregisterFifoReader() { m_fifoReader = nullptr; }
        EdPF::Fifo<GrainPlotData>* GetFifo() { return &m_fifo; };

    private:
        // This is a function used to accumulate the sample of all the active grains
        float AccumulateGrainOuput(int i);
        // This is where we hold a reference to the pooled grains
        EdPF::Grains::GrainPool<FGDSP::Grain>& m_pooledGrains;
        // This is where we keep a reference to all the smoothed values
        std::vector<juce::AudioBuffer<float>>& m_smoothedValues;
        double m_sampleRate;
        // The fifo that's used to push grain data to the GUI
        EdPF::Fifo<GrainPlotData> m_fifo;
        int m_totalNumberWritten;

        juce::Component* m_fifoReader;
        GrainPlotData m_newFifoData;

        juce::AudioPlayHead::CurrentPositionInfo& m_currentPositionInfo;
        
    };
    
}

#endif //!GRAIN_SCHEDULER_H_INCLUDED
