#include "AffineTransformsComponent.h"


//==============================================================================
AffineTransformsComponent::AffineTransformsComponent() 
    :
    m_valueTree(AffineTransformConstants::valueTreeID),
    m_sliders(AffineTransformConstants::NumParams),
    m_affineTransformsDisplay(m_valueTree)

{
    setSize(800, 500);
    CreateProperties();
}

AffineTransformsComponent::~AffineTransformsComponent()
{
}

//==============================================================================
void AffineTransformsComponent::paint(juce::Graphics& g)
{
}

void AffineTransformsComponent::resized()
{
    // Creating the Main Display for the affine transforms
    {
        auto area = getLocalBounds();
        area.removeFromBottom(AffineTransformConstants::sliderBankHeight);

        m_affineTransformsDisplay.setBounds(area);
        addAndMakeVisible(m_affineTransformsDisplay);
    }


    // Programatically positioning the sliders
    for (int slider = 0; slider < AffineTransformConstants::NumParams; ++slider)
    {
        auto area = getLocalBounds();
        auto leftScale = static_cast<float>(slider) / static_cast<float>(AffineTransformConstants::NumParams);
        auto rightScale = 1.0f - static_cast<float>(slider + 1) / static_cast<float>(AffineTransformConstants::NumParams);

        area.removeFromLeft(static_cast<int>(static_cast<float>(getWidth()) * leftScale));
        area.removeFromRight(static_cast<int>(static_cast<float>(getWidth()) * rightScale));
        area.removeFromTop(getHeight() - AffineTransformConstants::sliderBankHeight);

        m_sliders[slider].setBounds(area);
        addAndMakeVisible(m_sliders[slider]);
    }
}

void AffineTransformsComponent::CreateProperties()
{
    
    for (int slider = 0; slider < AffineTransformConstants::NumParams; ++slider)
    {
        // Let the display listen to the sliders so we can set the coefficients
        m_sliders[slider].getValueObject().addListener(&m_affineTransformsDisplay);
        // Set the ranges of the coefficients
        m_sliders[slider].setNormalisableRange(AffineTransformConstants::ParamRanges[slider]);
        m_sliders[slider].setValue(1.0f);
        // Store the properties in a tree so they're easily referable to
        m_valueTree.setProperty(AffineTransformConstants::ParameterIDS[slider], m_sliders[slider].getValueObject(), nullptr);
        // Make the sliders refer to the propertys underlying value objects within the tree
        m_sliders[slider].getValueObject().referTo(m_valueTree.getPropertyAsValue(AffineTransformConstants::ParameterIDS[slider], nullptr));
        
    }
}

