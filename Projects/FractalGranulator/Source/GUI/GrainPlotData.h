/*
  ==============================================================================

    GrainPlotData.h
    Created: 29 Aug 2022 3:58:17pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef GRAIN_PLOT_DATA_H_INCLUDED
#define GRAIN_PLOT_DATA_H_INCLUDED

#include "../FractalGranulatorCommon.h"

namespace FGDSP
{
    // This is the data that we'll push to the GUI, this can be modified to suit our needs
    class GrainPlotData
    {
    public:
        GrainPlotData();
        // To be called when the data is pushed from the DSP
        void Configure(float durationMs, float pitch, float distanceFromOriginScalar);
        // To be called everytime the GUI timer callback is called
        void IncrementDisplayLifeTime();
        // If the grain isn't alive we probably don't need to plot it...
        bool GetIsAlive() { return m_isAlive; }
        // X-Axis plot
        float GetDistanceFromOriginScalar() { return m_distanceFromOriginScalar; }
        // Size or gradient of the grain based on it's lifetime position
        float GetCurrentLifeTimePosition0to1();
        // Y-Axist plot
        float GetPitch() { return m_pitch; }
    private:
        float m_displayLifetime;
        float m_distanceFromOriginScalar;
        float m_pitch;
        float m_displayCurrentLifeTimePosition;
        float m_displayDelta;
        bool m_isAlive;
    };
}

#endif //!GRAIN_PLOT_DATA_H_INCLUDED
