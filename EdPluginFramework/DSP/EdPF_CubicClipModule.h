/*
  ==============================================================================

    EdPF_CubicClipModule.h
    Created: 30 Aug 2022 9:03:47pm
    Author:  Edward Gallyot

  ==============================================================================
*/

//
// Created by edgallyot on 07/03/2022.
//

#ifndef DISTORTED_CUBICMODULE_H
#define DISTORTED_CUBICMODULE_H

#include "EdPF_DistortionAlgorithms.h"

#include <JuceHeader.h>

namespace EdPF
{
    namespace DSP
    {
        namespace Distortion
        {
            class  CubicModule
            {
            public:
                CubicModule() = default;
                void prepare(double sampleRate, int maximumExpectedSamplesPerBlock);
                void process(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages, float& drive);
            private:
                juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedValue;
                float(&dsp) (float, float) = processCubicClip<float>;
            };
        }

    }
}


#endif //DISTORTED_CUBICMODULE_H