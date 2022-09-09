/*
  ==============================================================================

    EdPF_AudioVisualiser.cpp
    Created: 20 Aug 2022 9:28:28am
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "EdPF_AudioVisualiser.h"

EdPF::AudioVisualiser::AudioVisualiser(WavePlotDataGenerator& generator, int timerHz, int numPlotPoints, std::atomic<float>* meter) :
    m_generator(generator),
    m_numPlotPoints(numPlotPoints),
    m_readBuffer(numPlotPoints),
    m_plotData(numPlotPoints),
    m_writeIndex(0),
    m_meter(meter)
{
    m_generator.RegisterComponent(this);
    startTimerHz(timerHz);
}

EdPF::AudioVisualiser::~AudioVisualiser()
{
    m_generator.DeregisterComponent();
}

void EdPF::AudioVisualiser::paint(juce::Graphics& g)
{
    auto alpha = 0.5f + (0.5f * static_cast<float>(m_meter->load()));
    if (alpha >= 1.0f)
    {
        alpha = 1.0f;
    }
    g.setColour(FGConst::WaveFormColour.withAlpha(alpha));
    juce::Path p;
    auto height = static_cast<float>(getHeight());
    for (int i = 0; i < m_numPlotPoints; ++i)
    {
        if (i == 0)
        {
            p.startNewSubPath({ -1.0f, height / 2.0f });
        }

        int readIndex = (i + m_writeIndex) % m_numPlotPoints;
        auto x = static_cast<float>(i);
        auto yPositive = height * ((m_plotData[readIndex].GetPositiveSample() * 0.6f) + 0.6f);
        p.lineTo({x, yPositive });
    } 

    
    for (int i = m_numPlotPoints - 1; i != 0; --i)
    {
        if (i == m_numPlotPoints)
        {
            p.lineTo({ static_cast<float>(getWidth()) + 1.0f, height / 2.0f});
        }

        int readIndex = (i + m_writeIndex) % m_numPlotPoints;
        auto x = static_cast<float>(i);
        auto yNegative = height * ((m_plotData[readIndex].GetNegativeSample() * 0.6f) + 0.6f);
        p.lineTo({x, yNegative });
    }
    p.lineTo({ -1.0f, height / 2.0f });

    p.closeSubPath();

    g.fillPath(p);

    

    g.setColour(FGConst::GrainColour.withAlpha(alpha));
    for (int i = 0; i < m_numPlotPoints; ++i)
    {
        int readIndex = (i + m_writeIndex) % m_numPlotPoints;
        auto x = static_cast<float>(i);
        auto yPositive = height * ((m_plotData[readIndex].GetPositiveSample() * 0.6f) + 0.6f);
        auto yNegative = height * ((m_plotData[readIndex].GetNegativeSample() * 0.6f) + 0.6f);
        g.drawEllipse({x, yPositive, 1.0f, 1.0f}, 1.0f);
        g.drawEllipse({x, yNegative, 1.0f, 1.0f}, 1.0f);
    }

   
   
}

void EdPF::AudioVisualiser::timerCallback()
{
    int numPlotPointsRead = m_generator.GetFifo()->ReadFromFifo(m_readBuffer.data(), m_numPlotPoints);
    for (int i = 0; i < numPlotPointsRead; ++i)
    {
        m_plotData[m_writeIndex] = m_readBuffer[i];
        ++m_writeIndex;
        m_writeIndex = m_writeIndex % m_numPlotPoints;
    }
    repaint();
}
