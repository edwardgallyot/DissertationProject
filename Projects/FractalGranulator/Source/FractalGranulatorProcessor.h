/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef FRACTAL_GRANULAR_PROCESSOR_H
#define FRACTAL_GRANULAR_PROCESSOR_H

#include <JuceHeader.h>
#include "../../EdPluginFramework/Utils/EdPF_AudioProcessorEditor.h"
#include "../../EdPluginFramework/DSP/EdPF_DelayLine.h"
#include "../../EdPluginFramework/DSP/EdPF_Utils.h"
#include "../../EdPluginFramework/Utils/EdPF_Fifo.h"
#include "DSP/Granulator.h"


class FractalGranulatorAudioProcessor  : public EdPF::AudioProcessor               
{
public:
    FractalGranulatorAudioProcessor();
    ~FractalGranulatorAudioProcessor() override;

    void prepareToPlay(double, int) override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorValueTreeState::ParameterLayout CreateParameterLayout() override;

    juce::AudioProcessorEditor* createEditor() override;

    LinearDelayLine& GetDelayLine() { return m_delayLine; };

    FGDSP::Granulator& GetGranulator() { return m_granulator; }

    juce::Value GetCurrentOutputMeterValueObject() { return m_currentOutputMeter; }

private:
    void UpdateOutputValueMeter(juce::AudioBuffer<float>& buffer);
    juce::AudioBuffer<float> m_copyBuffer1;

    LinearDelayLine m_delayLine;
    FGDSP::Granulator m_granulator;

    // We want to keep track of the play head info so the scheduler knows whether to activate grains
    juce::AudioPlayHead::CurrentPositionInfo m_currentPositionInfo;

    // We will keep track of the current output meter
    juce::Value m_currentOutputMeter;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FractalGranulatorAudioProcessor)
};

#endif //!FRACTAL_GRANULAR_PROCESSOR_H