/*
  ==============================================================================

    FractalDisplayComponent.cpp
    Created: 3 Aug 2022 2:50:49pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "FractalDisplayComponent.h"

FGGUI::FractalDisplay::FractalDisplay(FractalGranulatorAudioProcessor& p) :
    m_processor(p),
    m_fifo(p.GetGranulator().GetSchedulerFifo()),
    m_plotData(FGConst::NumGrainPlotPoints),
    m_writeIndex(0),
    m_outputMeter(p.GetCurrentOutputMeter())
{
    m_processor.GetGranulator().RegisterFifoReader(this);
    startTimerHz(FGConst::GUITimerHz);
}

FGGUI::FractalDisplay::~FractalDisplay()
{
    m_processor.GetGranulator().DeregisterFifoReader();
}

void FGGUI::FractalDisplay::paint(juce::Graphics& g)
{
    auto alpha = 0.2f + (0.8f * static_cast<float>(m_outputMeter->load()));
    if (alpha >= 1.0f)
    {
        alpha = 1.0f;
    }
    g.setColour(juce::Colours::aquamarine.withAlpha(alpha));


    for (int i = 0; i < FGConst::NumGrainPlotPoints; ++i)
    {
        // PLOT THE GRAIN DATA AS IT COMES TO THE GUI
        // The plot data can mange it's own lifetime within the GUI
        // This way it doesn't need a constant stream of data from the DSP
        m_plotData[i].IncrementDisplayLifeTime();

        // We will only plot alive grains
        if (m_plotData[i].GetIsAlive())
        {
            float xPos = static_cast<float>(getWidth()) * ((m_plotData[i].GetDistanceFromOriginScalar()
                - FGConst::MinDistanceFromOriginScalar));
            float yPos = static_cast<float>(getHeight()) -
                (((m_plotData[i].GetPitch() - FGConst::MinimumPitch) / (FGConst::MaximumPitch - FGConst::MinimumPitch))) * static_cast<float>(getHeight());
            float thickness = 1.0f + (10.0f * m_plotData[i].GetCurrentLifeTimePosition0to1());
            g.drawEllipse
            (
                xPos,
                yPos,
                2.0f,
                2.0f,
                thickness
            );
        }
    }
}

void FGGUI::FractalDisplay::timerCallback()
{
    int numRead = m_fifo->ReadFromFifo(m_readBuffer.data(), 2000);
    for (int i = 0; i < numRead; ++i)
    {
        m_plotData[m_writeIndex] = m_readBuffer[i];
        ++m_writeIndex;
        m_writeIndex = m_writeIndex % FGConst::NumGrainPlotPoints;
    }
    repaint();
}
