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

class FractalGranulatorAudioProcessor  : public EdPF::AudioProcessor               
{
public:
    FractalGranulatorAudioProcessor();
    ~FractalGranulatorAudioProcessor() override;

    void prepareToPlay(double, int) override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorValueTreeState::ParameterLayout CreateParameterLayout() override;

    juce::AudioProcessorEditor* createEditor() override;

private:
    juce::AudioBuffer<float> m_copyBuffer1;

    using LinearDelayLine = EdPF::DSP::DelayLine<float, EdPF::DSP::LinearInterpolation>;
    LinearDelayLine m_delayLine;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FractalGranulatorAudioProcessor)
};

#endif //!FRACTAL_GRANULAR_PROCESSOR_H