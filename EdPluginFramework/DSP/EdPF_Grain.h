/*
  ==============================================================================

    EdPF_Grain.h
    Created: 22 Aug 2022 10:40:28pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_GRAIN_INCLUDED_H
#define EDPF_GRAIN_INCLUDED_H

#include <JuceHeader.h>
#include "EdPF_GrainSource.h"
#include "EdPF_GrainEnvelope.h"

namespace EdPF
{
    namespace Grains
    {
        // Provides an interface for a single grain.
        class Grain
        {
        public:
            Grain() : m_isActive(false), m_source(nullptr), m_duration(0), m_phase(0), m_envelope(nullptr)
            {
            }
            ~Grain()
            {
            }
            // The derived class will choose how the Grain is activated
            virtual void Activate(int duration, Envelope::Essence* envEssence, Source::Essence* srcEssence) = 0;
            // The derived calss will also choose how the griain is deactivated
            virtual void Deactivate() = 0;
            // This should be where the grain gives a sample to the scheduler
            virtual float ProcessSample() = 0;
            // Get the status of the grain
            bool GetIsActive() { return m_isActive; }
            // Set the status of the grain
            void SetIsActive(bool status) { m_isActive = status; }
            // Set the envelope type that we're going to use within the grain
            void SetEnvelope(std::unique_ptr<Envelope> envelope) { m_envelope = std::move(envelope); }
            // Set the source type that the grain is going to use
            void SetSource(std::unique_ptr<Source> source) { m_source = std::move(source); }
            // Initialise the envelope with an essence.
            void InitialiseEnvelope(Envelope::Essence* essence) { m_envelope->Init(essence); };
            // Initialise the source with it's essence
            void InitialiseSource(Source::Essence* essence) { m_source->Init(essence); }
            // Get the next sample from the envelope
            float GetNextEnvelopeSample() { return m_envelope->GetNextSample(); }
            // Get the next sample from the grain source
            float GetNextSourceSample() { return m_source->GetNextSample(); }
            // Get a pointer to the source data so we can modify parameters in real time
            Source* GetUnderlyingSource() { return m_source.get(); }
            // Set the duration
            void SetDuration(int duration) { m_duration = duration; }
            // Get the duration
            int GetDuration() { return m_duration; }
            // Get the current phase
            int GetPhase() { return m_phase; }
            // Increment the phase
            void IncrementPhase(int increment) { m_phase += increment; }
            // Reset the phase
            void ResetPhase() { m_phase = 0; }
        private:
            bool m_isActive;
            int m_phase;
            int m_duration;
            std::unique_ptr<Source> m_source;
            std::unique_ptr<Envelope> m_envelope;
        };
    }
}

#endif //!EDPF_GRAIN_INCLUDED_H
