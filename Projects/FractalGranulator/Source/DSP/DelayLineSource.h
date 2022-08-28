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
        struct Essence : public EdPF::Grains::Source::Essence
        {
            Essence(float grainDuration, float distanceFromPlayHead, int currentSample);
            void Configure() override;
            float GetDistanceFromPlayheadScalar() { return distanceFromPlayHeadScalar; };
            float GetGrainDuration() { return grainDuration; }
            int GetCurrentSampleInBlock() { return currentSampleInBlock; }
        private:
            float distanceFromPlayHeadScalar;
            float grainDuration;
            int currentSampleInBlock;
        };
        void Init(EdPF::Grains::Source::Essence* essence) override;     
    private:
        void IncrementSamplesProcessedInBlock();
        LinearDelayLine& m_delayLine;
        float m_distanceFromPlayheadScalar;
        float m_delayTimeSamples;
        float m_readIndex;
        float m_currentPhase;
        int m_samplesLeftToProcess;
        float m_grainDuration;

        float m_previousReadIndex;
    };
}
#endif //!DELAY_LINE_SOURCE_H_INCLUDED