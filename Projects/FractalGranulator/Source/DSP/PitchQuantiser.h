/*
  ==============================================================================

    PitchQuantiser.h
    Created: 29 Aug 2022 8:50:41pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef PITCH_QUANTISER_H_INCLUDED
#define PITCH_QUANTISER_H_INCLUDED

#include "../FractalGranulatorCommon.h"
#include <cmath>


//float closest(std::vector<float> const& vec, float value) {
//    auto const it = std::lower_bound(vec.begin(), vec.end(), value);
//    if (it == vec.end()) { return -1; }
//
//    return *it;
//}


namespace FGDSP
{
    class PitchQuantizer
    {
    public:

        PitchQuantizer() = default;
        ~PitchQuantizer() = default;

        void SetHarmony(FGConst::Harmony harmony) { m_harmony = harmony; }
        float GetQuantizedPitch(float grainPitch);
    private:
        float ClosestValueInVector(std::vector<float> const& vec, float value);
        FGConst::Harmony m_harmony;
    };
}


#endif //!PITCH_QUANTISER_H_INCLUDED