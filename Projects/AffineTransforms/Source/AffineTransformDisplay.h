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
                                public juce::Value::Listener
{
public:
    AffineTransformsDisplay(ValueTree& treeToReferTo);
    ~AffineTransformsDisplay();

    void valueChanged(Value&) override;

    void paint(juce::Graphics& g) override;

    Rectangle<float> getLocalFloatBounds();

private:
    ValueTree& m_valueTree;
};

#endif //!AFFINE_TRANSFORM_DISPLAY_H
