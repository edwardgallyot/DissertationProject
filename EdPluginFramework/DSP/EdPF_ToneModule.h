/*
  ==============================================================================

    EdPF_ToneModule.h
    Created: 30 Aug 2022 8:48:20pm
    Author:  Edward Gallyot

  ==============================================================================
*/

//
// Created by edgallyot on 08/03/2022.
//

#ifndef DISTORTED_TONEMODULE_H
#define DISTORTED_TONEMODULE_H

#include <JuceHeader.h>


namespace EdPF
{
    namespace DSP
    {
        using namespace juce::dsp;
        class ToneModule
        {
        public:
            ToneModule() = default;
            void prepare(double sampleRate, unsigned int samplesPerBlock, unsigned int numChannels, StateVariableFilter::Parameters<float>::Type filterType);
            void process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages, float cutoff);
        private:
            double sampleRate{ 0.0f };
            juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedValue;
            ProcessorDuplicator<StateVariableFilter::Filter<float>, StateVariableFilter::Parameters<float>> stateVariableFilter;
        };
    }
    
}

#endif //DISTORTED_TONEMODULE_H
