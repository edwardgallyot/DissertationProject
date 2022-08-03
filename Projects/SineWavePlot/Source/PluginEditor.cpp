/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SineWavePlotAudioProcessorEditor::SineWavePlotAudioProcessorEditor(SineWavePlotAudioProcessor& p)
    :
    AudioProcessorEditor(&p),
    audioProcessor(p),
    m_ampSlider(),
    m_freqSlider(),
    m_ampSliderAttachment
    (
        std::make_unique <SliderParameterAttachment>
        (
            *p.GetParameter(SWP::ParamIds[SWP::Param_Amp]), 
            m_ampSlider
        )
    ),
    m_freqSliderAttachment
    (
        std::make_unique<SliderParameterAttachment>
        (
           *p.GetParameter(SWP::ParamIds[SWP::Param_Freq]),
            m_freqSlider
        )
    ),
    m_sineWavePlot(p.GetFifo())

{
    setSize (400, 300);
    addChildComponent(m_ampSlider);
    addChildComponent(m_freqSlider);
    addChildComponent(m_sineWavePlot);
}

SineWavePlotAudioProcessorEditor::~SineWavePlotAudioProcessorEditor()
{
}

//==============================================================================
void SineWavePlotAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void SineWavePlotAudioProcessorEditor::resized()
{
    m_sineWavePlot.setBounds(0, 0, getWidth(), getHeight() / 2);
    m_ampSlider.setBounds(0, getHeight() / 2, getWidth() / 2, getHeight() / 2);
    m_freqSlider.setBounds(getWidth() / 2, getHeight() / 2, getWidth() / 2, getHeight() / 2);
    m_ampSlider.setVisible(true);
    m_freqSlider.setVisible(true);
    m_sineWavePlot.setVisible(true);
}
