/*
  ==============================================================================

    EdPF_WavePlotData.cpp
    Created: 7 Sep 2022 6:32:18pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "EdPF_WavePlotData.h"

EdPF::WavePlotData::WavePlotData() : m_negativeSample(0.0f), m_positiveSample(0.0f), m_isNewTimeSeries(true)
{
}

EdPF::WavePlotData::~WavePlotData()
{
}

void EdPF::WavePlotData::ProcessMonoSample(float sample)
{
    if (m_isNewTimeSeries)
    {
        if (sample > -1.0f && sample < 1.0f)
        {
            m_isNewTimeSeries = false;
            m_positiveSample = sample;
            m_negativeSample = sample;
        }
    }

    if (sample > m_positiveSample && sample < 1.0f)
    {
        m_positiveSample = sample;
    }
    if (sample < m_negativeSample && sample > -1.0f)
    {
        m_negativeSample = sample;
    }
}

void EdPF::WavePlotData::Reset()
{
    m_isNewTimeSeries = true;
    m_positiveSample = 0.0f;
    m_negativeSample = 0.0f;
}
