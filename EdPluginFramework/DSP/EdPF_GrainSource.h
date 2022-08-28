/*
  ==============================================================================

    EdPF_GrainSource.h
    Created: 28 Aug 2022 12:00:08pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef GRAIN_SOURCE_H_INCLUDED
#define GRAIN_SOURCE_H_INCLUDED

#include <JuceHeader.h>

namespace EdPF
{
    namespace Grains
    {
        class Source
        {
        public:
            virtual ~Source() = default;
            virtual float GetNextSample() = 0;

            // Provides an interface for a source essence
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
        };
    }
}
#endif //!GRAIN_SOURCE_H_INCLUDED
