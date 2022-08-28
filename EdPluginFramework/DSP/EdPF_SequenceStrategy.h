/*
  ==============================================================================

    EdPF_SequenceStrategy.h
    Created: 27 Aug 2022 6:04:40pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_SEQUENCE_STRATEGY_H_INCLUDED
#define EDPF_SEQUENCE_STRATEGY_H_INCLUDED

#include <vector>
#include <JuceHeader.h>

namespace EdPF
{
    namespace Grains
    {
        class SequenceStrategy
        {
        public:
            virtual float GetNextDuration() = 0;
            virtual int GetNextInterOnset() = 0;
        private:
        };
    }
}

#endif //!EDPF_SEQUENCE_STRATEGY_H_INCLUDED
