/*
  ==============================================================================

    EdPF_WavePlotGenerator.h
    Created: 7 Sep 2022 6:39:04pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_WAVEPLOT_DATA_GENERATOR_H_INCLUDED
#define EDPF_WAVEPLOT_DATA_GENERATOR_H_INCLUDED

#include "EdPF_Fifo.h"
#include "EdPF_WavePlotData.h"
#include "../DSP/EdPF_Utils.h"
#include "JuceHeader.h"

namespace EdPF
{
    class WavePlotDataGenerator
    {
    public:
        WavePlotDataGenerator(float timeMs, int numPlotPoints);
        ~WavePlotDataGenerator() = default;
        // Sets the sample rate and calculates the samples per update
        void SetSampleRate(double sampleRate);
        // Processes one mono sample
        void ProcessSample(float sample);
        //TODO: add support for stereo
        
        // Registers a GUI Component
        void RegisterComponent(juce::Component* component);
        // Deregisters a GUI Component
        void DeregisterComponent() { m_component = nullptr; }

        Fifo<WavePlotData>* GetFifo() { return &m_fifo; }

    private:
        int m_numSamplesPerUpdate;
        int m_numSamplesLeftToProcess;
        int m_numPlotPoints;
        // The registered component to read
        juce::Component* m_component;
        
        Fifo<WavePlotData> m_fifo;

        WavePlotData m_plotData;
        
        double m_sampleRate;
        float m_timeMs;
    };
}

#endif //!EDPF_WAVEPLOT_DATA_GENERATOR_H_INCLUDED