/*
  ==============================================================================

    SineWavePlot.h
    Created: 28 Jul 2022 7:02:12pm
    Author:  Edward Gallyot

  ==============================================================================
*/


#ifndef SINEWAVEPLOT_H
#define SINEWAVEPLOT_H

#include <JuceHeader.h>
#include "EdPF_Fifo.h"

class SineWavePlot : public Component,
                    public Timer
{
public:
    SineWavePlot(EdPF::Fifo<float>&);
    void paint(Graphics& g) override;
    void resized() override;
    void timerCallback() override;
private:

    std::vector<float> m_readBuffer;
    std::vector<Range<float>> m_plotData;
    int m_plotDataWritePosition;
    EdPF::Fifo<float>& m_fifo;
};

#endif // !SINEWAVEPLOT_H
