/*
  ==============================================================================

    EdPF_WavePlotData.cpp
    Created: 7 Sep 2022 6:32:18pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "EdPF_WavePlotData.h"

EdPF::WavePlotData::WavePlotData() : m_negativeSample(0.0f), m_positiveSample(0.0f)
{
}

EdPF::WavePlotData::~WavePlotData()
{
}

void EdPF::WavePlotData::ProcessMonoSample(float sample)
{
    if (sample > m_positiveSample)
    {
        m_positiveSample = sample;
    }
    if (sample < m_negativeSample)
    {
        m_negativeSample = sample;
    }
}

void EdPF::WavePlotData::Reset()
{
    m_positiveSample = 0.0f;
    m_negativeSample = 0.0f;
}
