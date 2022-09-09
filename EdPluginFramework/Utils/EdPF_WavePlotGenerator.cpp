/*
  ==============================================================================

    EdPF_WavePlotGenerator.cpp
    Created: 7 Sep 2022 6:39:04pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "EdPF_WavePlotGenerator.h"

EdPF::WavePlotDataGenerator::WavePlotDataGenerator(float timeMs, int numPlotPoints) :
    m_component(nullptr),
    m_numSamplesPerUpdate(0),
    m_numPlotPoints(numPlotPoints),
    m_fifo(numPlotPoints + 1),
    m_numSamplesLeftToProcess(0),
    m_sampleRate(0.0),
    m_timeMs(timeMs)
{
}

void EdPF::WavePlotDataGenerator::SetSampleRate(double sampleRate)
{
    m_sampleRate = sampleRate;
    auto timeInSamples = DSP::Utils::MsToSamples(m_timeMs, static_cast<float>(sampleRate));
    m_numSamplesPerUpdate = static_cast<int>(timeInSamples / static_cast<float>(m_numPlotPoints));   
}

void EdPF::WavePlotDataGenerator::ProcessSample(float sample)
{
    // Process a sample from the plot data
    m_plotData.ProcessMonoSample(sample);
    // If we have a registered component for the fifo then we will add to its
    if (m_component != nullptr)
    {
        if (--m_numSamplesLeftToProcess <= 0)
        {
            m_fifo.AddToFifo(&m_plotData, 1);
            m_plotData.Reset();
            m_numSamplesLeftToProcess = m_numSamplesPerUpdate;
        }
    }
}

void EdPF::WavePlotDataGenerator::RegisterComponent(juce::Component* component)
{
    m_component = component;
}


