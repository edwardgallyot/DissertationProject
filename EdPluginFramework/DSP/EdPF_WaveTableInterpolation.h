/*
  ==============================================================================

    EdPF_WaveTableInterpolation.h
    Created: 3 Aug 2022 2:54:10pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_WAVETABLE_INTERPOLATION_H
#define EDPF_WAVETABLE_INTERPOLATION_H

#include <JuceHeader.h>

namespace EdPF
{
    namespace DSP
    {
        struct LinearInterpolation         
        {
            template <typename FloatType>
            FloatType GetSample(FloatType index, juce::AudioBuffer<FloatType>& buffer) 
            {
                // Let's make some assertions to check we haven't misused the class.
                jassert(index <= static_cast<FloatType>(buffer.getNumSamples()));
                jassert(buffer.getNumChannels() == 1);

                // Let's find the indicies of the channels
                int a = static_cast<int>(index);
                int b = (a + 1) % buffer.getNumSamples();

                // Find two values to interpolate between
                FloatType xN = buffer.getSample(0, a);
                FloatType xNPlusOne = buffer.getSample(0, b);

                // Let's find the fractional part
                FloatType frac = index - a;

                // Return the result using the linear interpolation algorithm.
                return xNPlusOne * frac + (xN * (static_cast<FloatType>(1.0f) - frac));
            };

        };

        struct CubicInterpolation 
        {
            template<typename FloatType>
            FloatType GetSample(FloatType index, juce::AudioBuffer<FloatType>& buffer)
            {
                // TODO: Try out some cubic interpolation.
                return index;
            }
        };
    }
}

#endif //!EDPF_WAVETABLE_INTERPOLATION_H
