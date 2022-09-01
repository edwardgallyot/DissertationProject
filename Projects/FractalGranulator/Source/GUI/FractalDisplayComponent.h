/*
  ==============================================================================

    FractalDisplayComponent.h
    Created: 3 Aug 2022 2:50:49pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef FRACTAL_DISPLAY_COMPONENT_H
#define FRACTAL_DISPLAY_COMPONENT_H

#include "JuceHeader.h"
#include "../FractalGranulatorProcessor.h"
#include "../FractalGranulatorCommon.h"
#include "GrainPlotData.h"

namespace FGGUI
{
    class FractalDisplay : public juce::Component, 
                           public juce::Timer
    {
    public:
        FractalDisplay(FractalGranulatorAudioProcessor& p);
        ~FractalDisplay();
        void paint(juce::Graphics& g);

        void timerCallback() override;

    private:
        FractalGranulatorAudioProcessor& m_processor;
        std::array<FGDSP::GrainPlotData, 2000> m_readBuffer;
        EdPF::Fifo<FGDSP::GrainPlotData>* m_fifo;
        std::vector<FGDSP::GrainPlotData> m_plotData;
        int m_writeIndex;

        std::atomic<float>* m_outputMeter;
    };
} 

#endif //!FRACTAL_DISPLAY_COMPONENT_H
