/*
  ==============================================================================

    EdPF_WavePlotData.h
    Created: 7 Sep 2022 6:32:18pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_WAVEPLOT_DATA_H_INCLUDED
#define EDPF_WAVEPLOT_DATA_H_INCLUDED

namespace EdPF
{
    class WavePlotData
    {
    public:
        WavePlotData();
        ~WavePlotData();

        void ProcessMonoSample(float sample);
        void Reset();

        float GetPositiveSample() { return m_positiveSample; }
        float GetNegativeSample() { return m_negativeSample; }
    private:
        float m_negativeSample;
        float m_positiveSample;
    };
}

#endif //!EDPF_WAVEPLOT_DATA_H_INCLUDED
