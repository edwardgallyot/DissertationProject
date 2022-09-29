/*
  ==============================================================================

    GrainPlotData.cpp
    Created: 29 Aug 2022 3:58:18pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "GrainPlotData.h"

FGDSP::GrainPlotData::GrainPlotData() :
    m_displayLifetime(0.1f),
    m_distanceFromOriginScalar(0.0f),
    m_pitch(0.0f),
    m_displayCurrentLifeTimePosition(0.0f),
    m_displayDelta(0.0f)
{
}

void FGDSP::GrainPlotData::Configure(float durationMs, float pitch, float distanceFromOrigin)
{
    m_distanceFromOriginScalar = distanceFromOrigin;
    m_displayLifetime = durationMs / 1000.0f;
    m_displayDelta = 1.0f / static_cast<float>(FGConst::GUITimerHz) * pitch;
    m_displayCurrentLifeTimePosition = 0.0f;
    m_pitch = pitch;
    m_isAlive = true;
}

void FGDSP::GrainPlotData::IncrementDisplayLifeTime()
{
    m_displayCurrentLifeTimePosition += m_displayDelta;
    if (m_displayCurrentLifeTimePosition >= m_displayLifetime)
    {
        m_isAlive = false;
    }
}

float FGDSP::GrainPlotData::GetCurrentLifeTimePosition0to1()
{
    auto halfLifeTime = m_displayLifetime / 2.0f;
    if (m_displayCurrentLifeTimePosition <= halfLifeTime)
    {
        return m_displayCurrentLifeTimePosition / halfLifeTime;
    }
    return (m_displayCurrentLifeTimePosition - m_displayLifetime) / halfLifeTime;
}
