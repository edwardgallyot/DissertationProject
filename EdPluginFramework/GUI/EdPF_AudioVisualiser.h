/*
  ==============================================================================

    EdPF_AudioVisualiser.h
    Created: 20 Aug 2022 9:28:28am
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_AUDIO_VISUALISER_H 
#define EDPF_AUDIO_VISUALISER_H

#include "JuceHeader.h"
#include "../Utils/EdPF_Fifo.h"
#include "../Utils/EdPF_WavePlotData.h"
#include "../Utils/EdPF_WavePlotGenerator.h"
#include "../../FractalGranulator/Source/FractalGranulatorCommon.h"

namespace EdPF
{
    class AudioVisualiser : public juce::Component,
                            public juce::Timer
    {
    public:
        AudioVisualiser(WavePlotDataGenerator& m_generator, int timerHz, int numPlotPoints, std::atomic<float>* meter);
        ~AudioVisualiser();
    private:
        void paint(juce::Graphics& g) override;
        void timerCallback() override;

        WavePlotDataGenerator& m_generator;
        int m_numPlotPoints;
        std::vector<WavePlotData> m_readBuffer;
        std::vector<WavePlotData> m_plotData;
        int m_writeIndex;
        std::atomic<float>* m_meter; 
    };
}

#endif //!EDPF_AUDIO_VISUALISER_H
