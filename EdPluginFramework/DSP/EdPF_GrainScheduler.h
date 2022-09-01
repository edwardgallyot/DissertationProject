/*
  ==============================================================================

    EdPF_GrainScheduler.h
    Created: 22 Aug 2022 10:40:01pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_GRAIN_SCHEDULER_H_INCLUDED
#define EDPF_GRAIN_SCHEDULER_H_INCLUDED

#include "EdPF_GrainPool.h"
#include "EdPF_Grain.h"
#include "EdPF_SequenceStrategy.h"

namespace EdPF
{
    namespace Grains
    {
        // Provides some of the logic to schedule grains based on a sequence strategy.
        class Scheduler
        {
        public:
            Scheduler();
            virtual ~Scheduler() = default;
            // Provides an interface to synthesise a sample using a scheduler.
            virtual float SythesiseNextSample(int i) = 0;

            void IncrementSample();
            
            bool ShouldActivateGrain() { return m_shouldActivateGrain; };

            float GetNextDuration() { return m_sequenceStrategy->GetNextDuration(); }

            // You'll want to cast this to your derived sequence strategy
            SequenceStrategy* GetUnderlyingSequenceStrategy() { return m_sequenceStrategy.get(); }

            void SetSequenceStrategy(std::unique_ptr<SequenceStrategy> sequenceStrategy) { m_sequenceStrategy = std::move(sequenceStrategy); };
        private:
            int m_nextOnset;
            float m_nextDuration;
            bool m_shouldActivateGrain;
            std::unique_ptr<SequenceStrategy> m_sequenceStrategy;
        };
    }
}

#endif // !EDPF_GRAIN_SCHEDULER_H_INCLUDED
