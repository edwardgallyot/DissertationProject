/*
  ==============================================================================

    SineWavePlot.cpp
    Created: 28 Jul 2022 7:02:12pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "SineWavePlot.h"

SineWavePlot::SineWavePlot(EdPF::Fifo<float>& fifoRef) : 
    m_fifo(fifoRef),
    m_plotDataWritePosition(0)
{
    m_readBuffer.resize(1024);
    startTimerHz(60);
}

void SineWavePlot::paint(Graphics& g)
{
    g.fillAll(Colours::beige);

    g.setColour(Colours::orangered);
    Path p;

    auto heightF = static_cast<float>(getHeight());
    auto widthF = static_cast<float>(getWidth());

    p.startNewSubPath({ widthF, heightF / 2.0f });

    // Draw the top of the range
    for (auto i = static_cast<int>(m_plotData.size() - 1); i > 1; --i)
    {
        auto localPoint = Point<float>(i, (heightF / 2.0f) + (m_plotData[i].getEnd() * (heightF / 2.0f)));
        p.lineTo(localPoint);
    }
    p.lineTo(0.0f, heightF / 2.0f);

   
    p.closeSubPath();
    g.fillPath(p);
    p.startNewSubPath({ 0.0f, heightF / 2.0f });

    // Draw the bottom of the range
    for (auto i = 0; i < static_cast<int>(m_plotData.size()); ++i)
    {
        auto localPoint = Point<float>(i, (heightF / 2.0f) + (m_plotData[i].getStart() * (heightF / 2.0f)));
        p.lineTo(localPoint);
    }

    p.lineTo({ widthF, heightF / 2.0f });
    p.closeSubPath();
   
    g.fillPath(p);
}

void SineWavePlot::resized()
{
    m_plotData.resize(getWidth());
}

void SineWavePlot::timerCallback()
{
    m_fifo.ReadFromFifo(m_readBuffer);

    float sum = 0.0f;
    float averageMultiplier = 1.0f / static_cast<float>(m_readBuffer.size());

    float maxMagnitude = 0.0f;
    for (int i = 0; i < m_readBuffer.size(); ++i)
    {
        if (i == 0)
        {
            m_plotData[m_plotDataWritePosition] = { m_readBuffer[i] , m_readBuffer[i] };
        }
        else
        {
            m_plotData[m_plotDataWritePosition].getUnionWith(m_readBuffer[i]);
        }
    }
    m_plotDataWritePosition += 1;
    m_plotDataWritePosition %= static_cast<int>(m_plotData.size());
    
    repaint();
}
