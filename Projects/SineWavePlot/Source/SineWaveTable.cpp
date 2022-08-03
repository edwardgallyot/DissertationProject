/*
  ==============================================================================

    SineWaveTable.cpp
    Created: 28 Jul 2022 6:57:56pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "SineWaveTable.h"

SineWaveTable::SineWaveTable(int tableSize) :
    m_table(1, tableSize),
    m_readHead(0.0f),
    m_sampleRate(0.0f)
{
    CreateTable();
}

void SineWaveTable::PrepareToPlay(double sampleRate, int samplesPerBlock)
{
    m_freq.reset(sampleRate, 0.1);
    m_amp.reset(sampleRate, 0.1);
    m_sampleRate = static_cast<float>(sampleRate);
}

void SineWaveTable::ProcessBlock(AudioBuffer<float>& targetBuffer)
{
    for (int sample = 0; sample < targetBuffer.getNumSamples(); ++sample)
    {
        m_readHead += m_freq.getNextValue();
        // Wrap Around the Buffer
        m_readHead = std::fmod(m_readHead, static_cast<float>(m_table.getNumSamples()));

        // Set the output sample
        float outSample = GetInterpolatedSample(m_readHead) * m_amp.getNextValue();
        for (int channel = 0; channel < targetBuffer.getNumChannels(); ++channel)
        {   
            targetBuffer.setSample(channel, sample, outSample);
        }

    }
}

void SineWaveTable::SetFreq(float frequency)
{
    frequency = (frequency / m_sampleRate) * static_cast<float>(m_table.getNumSamples());
    m_freq.setTargetValue(frequency);
}

void SineWaveTable::SetAmp(float amplitude)
{
    m_amp.setTargetValue(amplitude);
}

void SineWaveTable::CreateTable()
{
    for (int sample = 0; sample < m_table.getNumSamples(); ++sample)
    {
        auto twoPiTimesN = static_cast<float>(sample) / static_cast<float>(m_table.getNumSamples()) * MathConstants<float>::twoPi;
        auto x_n = std::sin(twoPiTimesN);
        m_table.setSample(0, sample, x_n);
    }
}

float SineWaveTable::GetInterpolatedSample(float fractionalIndex)
{
    // Linear Interpolation
    auto floorIndex = static_cast<int>(fractionalIndex);
    int floorIndexPlusOne = (floorIndex + 1) % m_table.getNumSamples();

    //y(n) = x(n − [M + 1])frac + x(n − M)(1 − frac)
    auto frac = fractionalIndex - static_cast<float>(floorIndex);
    float xNMPlusOne = m_table.getSample(0, floorIndexPlusOne);
    float xNM = m_table.getSample(0, floorIndex);
    
    // y[n]
    return xNMPlusOne * frac + (xNM * (1.0f - frac));
}

