/*
  ==============================================================================

    EdPF_Granulator.h
    Created: 22 Aug 2022 10:38:50pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_GRANULATOR_H_INCLUDED
#define EDPF_GRANULATOR_H_INCLUDED

#include "EdPF_GrainScheduler.h"
#include "EdPF_GrainPool.h"

namespace EdPF
{
    namespace Grains
    {
        class Granulator
        {
        public:
            virtual void processSamples(juce::AudioBuffer<float> buffer) = 0;
        private:
        };
    }
} 

#endif //!EDPF_GRANULATOR_H_INCLUDED
