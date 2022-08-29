/*
  ==============================================================================

    DelayLineSource.h
    Created: 25 Aug 2022 10:23:29pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef DELAY_LINE_SOURCE_H_INCLUDED
#define DELAY_LINE_SOURCE_H_INCLUDED

#include "../../EdPluginFramework/DSP/EdPF_DelayLine.h"
#include "../../EdPluginFramework/DSP/EdPF_GrainSource.h"


namespace FGDSP
{
    class DelayLineSource : public EdPF::Grains::Source
    {
    public:
        DelayLineSource(LinearDelayLine& delayLine);

        void SetDelayTimeSamples(float delayTimeSamples);

        float GetNextSample() override;

        // This esseence is how the delay line source will be intialised
        struct Essence : public EdPF::Grains::Source::Essence
        {
            Essence(float duration, float distanceFromPlayHead, int currentSample, float pitch);
            void Configure() override;
            float GetDistanceFromPlayheadScalar() { return distanceFromPlayHeadScalar; };
            int GetCurrentSampleInBlock() { return currentSampleInBlock; }
            float GetGrainPitch() { return grainPitch; }
            float GetDelayTime() { return delayTime; }
            float GetGrainDuration() { return grainDuration; }
        private:
            float distanceFromPlayHeadScalar;
            int currentSampleInBlock;
            float grainPitch;
            float delayTime;
            float grainDuration;
        };
        void Init(EdPF::Grains::Source::Essence* essence) override;     
    private:
        void IncrementSamplesProcessedInBlock();
        LinearDelayLine& m_delayLine;
        float m_distanceFromPlayheadScalar;
        float m_delayTimeSamples;
        float m_readIndex;
        int m_samplesLeftToProcess;
        float m_pitch;
        float m_phase;
        float m_durationInSamples;
    };
}
#endif //!DELAY_LINE_SOURCE_H_INCLUDED