/*
  ==============================================================================

    PitchQuantiser.cpp
    Created: 29 Aug 2022 8:50:41pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "PitchQuantiser.h"

float FGDSP::PitchQuantizer::GetQuantizedPitch(float grainPitch)
{
    return ClosestValueInVector(*FGConst::Harmonies[m_harmony], grainPitch);
}

float FGDSP::PitchQuantizer::ClosestValueInVector(std::vector<float> const& vec, float value)
{
    // Algorithm for finding the closest element in a sorted vector from here.
    //https://stackoverflow.com/questions/62226032/c-lower-bound-to-search-for-the-closest-element-to-a-target-value

    auto attempt = lower_bound(vec.begin(), vec.end(), value);

    auto found = 
        attempt == vec.end() ?
        vec.end() - 1 :   // last element is closest
        attempt == vec.begin() ?
        vec.begin() :  // first element is closest
        abs(*attempt - value) < abs(*(attempt - 1) - value) ?
        attempt : attempt - 1; // somewhere in the middle, then the previous element could be closer

    return *found;

}
