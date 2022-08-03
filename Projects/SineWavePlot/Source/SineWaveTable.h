/*
  ==============================================================================

    SineWaveTable.h
    Created: 28 Jul 2022 6:57:56pm
    Author:  Edward Gallyot

  ==============================================================================

  */
#ifndef SINEWAVETABLE_H
#define SINEWAVETABLE_H

#include <JuceHeader.h>

class SineWaveTable
{
public:
    SineWaveTable(int tableSize);
    void PrepareToPlay(double sampleRate, int samplesPerBlock);
    void ProcessBlock(AudioBuffer<float>&);

    void SetFreq(float frequency);
    void SetAmp(float amplitude);
    
private:
    void CreateTable();
    float GetInterpolatedSample(float fractionalIndex);

    AudioBuffer<float> m_table;
    SmoothedValue<float> m_freq;
    SmoothedValue<float> m_amp;

    float m_sampleRate;
    float m_readHead;
};

#endif //!SINEWAVEPLOT_H