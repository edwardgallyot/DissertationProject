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
#include "../../EdPluginFramework/DSP/EdPF_ToneModule.h"
#include "../../EdPluginFramework/DSP/EdPF_CubicClipModule.h"
#include "../../EdPluginFramework/Utils/EdPF_WavePlotGenerator.h"
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

    std::atomic<float>* GetCurrentOutputMeter() { return &m_currentOutputMeter; }
    std::atomic<float>* GetCurrentInputMeter() { return &m_currentInputMeter; }
    std::atomic<float>* GetCurrentGranulatorMeter() { return &m_granulationMeter; }

    EdPF::WavePlotDataGenerator& GetInputWavePlotGenerator() { return m_inputWaveGenerator; }
    EdPF::WavePlotDataGenerator& GetOuputWavePlotGenerator() { return m_outputWaveGenerator; }

private:
    // ATOMIC UI METER
    //==============================================
    void UpdateSimpleMeter(std::atomic<float>* meter, juce::AudioBuffer<float>& buffer);
    // We will keep track of the current max output per block so the UI can update pretty colours
    std::atomic<float> m_currentOutputMeter;
    std::atomic<float> m_currentInputMeter;
    std::atomic<float> m_granulationMeter;

    // DRY INPUT COPY BUFFER
    // ============================================
    juce::AudioBuffer<float> m_dryBuffer;

    // COPY BUFFERS FOR DELAY
    //=============================================
    juce::AudioBuffer<float> m_feedbackOutputBuffer;
    juce::AudioBuffer<float> m_delayInputBuffer;
    
    // LINEARLY INTERPOLATED DELAY LINE
    //=============================================
    LinearDelayLine m_delayLine;

    // GRANULATOR
    //=============================================
    FGDSP::Granulator m_granulator;

    // JUCE COMPRESSOR FOR THE FINAL OUT
    //=============================================
    juce::dsp::Compressor<float> m_compressor;
    
    // PLAY HEAD INFO
    // ============================================
    juce::AudioPlayHead::CurrentPositionInfo m_currentPositionInfo;

    // FILTERS
    // ============================================
    EdPF::DSP::ToneModule m_lowPass;
    EdPF::DSP::ToneModule m_highPass;
    
    // DISTORTION
    // ============================================
    EdPF::DSP::Distortion::CubicModule m_distortion;

    // INPUT/OUTPUT FIFO
    // ============================================
    EdPF::WavePlotDataGenerator m_inputWaveGenerator;
    EdPF::WavePlotDataGenerator m_outputWaveGenerator;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FractalGranulatorAudioProcessor)
};

#endif //!FRACTAL_GRANULAR_PROCESSOR_H