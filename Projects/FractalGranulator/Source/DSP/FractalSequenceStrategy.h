/*
  ==============================================================================

    FractalSequenceStrategy.h
    Created: 29 Aug 2022 6:20:42pm
    Author:  Edward Gallyot

  ==============================================================================
*/


#ifndef FRACTAL_SEUQUENCE_STRATEGY_H_INCLUDED
#define FRACTAL_SEUQUENCE_STRATEGY_H_INCLUDED

#include "../../EdPluginFramework/DSP/EdPF_SequenceStrategy.h"
#include "../../EdPluginFramework/DSP/EdPF_Utils.h"
#include "../FractalGranulatorCommon.h"
#include "JuceHeader.h"

namespace FGDSP
{
    class FractalSequenceStrategy : public EdPF::Grains::SequenceStrategy
    {
    public:
        FractalSequenceStrategy();

        int GetNextInterOnset() override;
        float GetNextDuration() override;
        float GetNextDistanceFromPlayheadScalar();
        float GetNextPitch();
        void SetSampleRate(double sampleRate) { m_sampleRate = sampleRate; }
        void DoChaosGame();
        void SetComplexityScalar(float complexity) { m_complexityScalar = 1.0f - complexity; }
        void SetShapeOne(float shape) { m_shape1 = shape; };
        void SetShapeTwo(float shape) { m_shape2 = shape; };
        void SetShapeThree(float shape) { m_shape3 = shape; };
        void SetShapeFour(float shape) { m_shape4 = shape; };

        bool CheckPointsBounds();
    private:
        juce::Random m_random;
        double m_sampleRate;
        float m_complexityScalar;
        
        float m_shape1;
        float m_shape2;
        float m_shape3;
        float m_shape4;

        juce::Point<float> m_currentPoint;
        std::vector<juce::Point<float>> m_serpinskiBounds;
    };
}

#endif //!FRACTAL_SEUQUENCE_STRATEGY_H_INCLUDED
