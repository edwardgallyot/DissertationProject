/*
  ==============================================================================

    EdPF_GrainEnvelope.h
    Created: 28 Aug 2022 12:25:13pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef GRAIN_ENVELOPE_H_INCLUDED
#define GRAIN_ENVELOPE_H_INCLUDED

namespace EdPF
{
    namespace Grains
    {
        // This is an interface for an Envelope object
        class Envelope
        {
        public:
            Envelope() : m_duration(0) {};
            virtual ~Envelope() = default;
            // An envelope will need to provide a sample at each point in it's duration
            virtual float GetNextSample() = 0;
            
            // Provides an interface for an envelope essence
            struct Essence
            {
                // You'll want to override this method to configure your envelope with the correct parameters
                virtual void Configure() = 0;
                // Put your derived classes parameters below....
                // float m_amplitude;  for instance
            };
            // You'll also want to override this to initialise your envelope with the essence
            // In practice you will need to dynamic_cast<DerivedType>(essence) to use it with your
            // your own derived essence.
            virtual void Init(Essence* essence) = 0;
            
            int GetDuration() { return m_duration; }
            void SetDuration(int duration) { m_duration = duration; }
        private:
            int m_duration;
        };
    }
}

#endif //!GRAIN_ENVELOPE_H_INCLUDED
