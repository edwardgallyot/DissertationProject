#pragma once

#include <JuceHeader.h>
#include "AffineTransformDisplay.h"
#include "AffineTransformConstants.h"


class AffineTransformsComponent : public juce::Component
{
public:
    //==============================================================================
    AffineTransformsComponent();
    ~AffineTransformsComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void CreateProperties();

private:

    std::vector<juce::Slider> m_sliders;
    juce::ValueTree m_valueTree;

    AffineTransformsDisplay m_affineTransformsDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AffineTransformsComponent)
};
