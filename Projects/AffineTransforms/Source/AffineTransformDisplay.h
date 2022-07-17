/*
  ==============================================================================

    AffineTransformComponent.h
    Created: 16 Jul 2022 7:37:49pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef AFFINE_TRANSFORM_DISPLAY_H
#define AFFINE_TRANSFORM_DISPLAY_H

#include <JuceHeader.h>
#include "AffineTransformConstants.h"


class AffineTransformsDisplay : public juce::Component,
                                public juce::Value::Listener,
                                public juce::Timer      
{
public:
    AffineTransformsDisplay(ValueTree& treeToReferTo);
    ~AffineTransformsDisplay();

    void valueChanged(Value&) override;

    void paint(juce::Graphics& g) override;

    void resized() override;

    void timerCallback() override;

    Rectangle<float> GetLocalFloatBounds();

private:
    ValueTree& m_valueTree;

    void StoreScreenData();
    bool AssertResizeBounds();
    void InitialiseSerpinkskiBounds();
    void ChaosGame();

    const int m_numChaosGamePoints;

    // This vector must be resized within the resize function!!
    std::vector<std::vector<Point<float>>> m_screenData;
    std::vector<Point<float>> m_serpinskiBounds;
    std::vector<Point<float>> m_serpinskiPoints;
};

#endif //!AFFINE_TRANSFORM_DISPLAY_H
