/*
  ==============================================================================

    EdPF_AudioProcessor.h
    Created: 3 Aug 2022 2:51:28pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_AUDIO_PROCESSOR_H
#define EDPF_AUDIO_PROCESSOR_H

#include <JuceHeader.h>

namespace EdPF
{
    class AudioProcessor : public juce::AudioProcessor
                           
    {
    public: 
        //==============================================================================
        AudioProcessor();
        ~AudioProcessor() override;
        

        //==============================================================================
        void prepareToPlay(double sampleRate, int samplesPerBlock) override;
        void releaseResources() override;

        #ifndef JucePlugin_PreferredChannelConfigurations
        bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
        #endif

        void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

        //==============================================================================
        virtual juce::AudioProcessorEditor* createEditor() override;
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
        void setCurrentProgram(int index) override;
        const juce::String getProgramName(int index) override;
        void changeProgramName(int index, const juce::String& newName) override;

        //==============================================================================
        void getStateInformation(juce::MemoryBlock& destData) override;
        void setStateInformation(const void* data, int sizeInBytes) override;

        //juce::AudioProcessorValueTreeState m_apvts;
    };
}

#endif // !EDPF_AUDIO_PROCESSOR_H

