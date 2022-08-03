#ifndef PLUGIN_PROCESSOR_H
#define PLUGIN_PROCESSOR_H

#include <JuceHeader.h>
#include "SineWaveTable.h"
#include "SineWavePlotCommon.h"
#include "EdPF_Fifo.h"

//==============================================================================
/**
*/
class SineWavePlotAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    SineWavePlotAudioProcessor();
    ~SineWavePlotAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    RangedAudioParameter* GetParameter(StringRef ParamId);

    EdPF::Fifo<float>& GetFifo() { return m_fifo; };

private:
    AudioProcessorValueTreeState::ParameterLayout CreateParameterLayout();

    SineWaveTable m_sineWave;
    AudioProcessorValueTreeState m_params;

    EdPF::Fifo<float> m_fifo;
    std::vector<float> m_tmpBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SineWavePlotAudioProcessor)
};

#endif //!PLUGIN_PROCESSOR_H
