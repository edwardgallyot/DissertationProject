/*
  ==============================================================================

    EdPF_Granulator.h
    Created: 18 Sep 2022 5:07:24pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_GRANULATOR_H_INCLUDED
#define EDPF_GRANULATOR_H_INCLUDED

#include <JuceHeader.h>

namespace EdPF
{
    class Granulator
    {
    public:
        virtual ~Granulator() = default;
        virtual void PrepareToPlay(double sampleRate, int numSamplesExpected) = 0;
        virtual void ProcessSamples(juce::AudioBuffer<float>& buffer) = 0;
    };
	
} //!EdPF

#endif //!EDPF_GRANULATOR_H_INCLUDED
