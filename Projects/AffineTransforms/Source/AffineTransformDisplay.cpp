/*
  ==============================================================================

    AffineTransformComponent.cpp
    Created: 16 Jul 2022 7:37:49pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#include "AffineTransformDisplay.h"
 

AffineTransformsDisplay::AffineTransformsDisplay(ValueTree& treeToReferTo)
    : 
    m_valueTree(treeToReferTo)
{   
    
}

AffineTransformsDisplay::~AffineTransformsDisplay()
{

}

void AffineTransformsDisplay::valueChanged(Value& value)
{
    repaint();
}

void AffineTransformsDisplay::paint(juce::Graphics& g)
{
    // We create blocks so we can seperate out the processes
    {
        Path path;
        path.addRectangle(getLocalBounds());

        g.setColour(juce::Colours::darkcyan);

        g.fillPath(path);
        path.clear();
    }

    {
        using namespace AffineTransformConstants;
        Path path;
        auto  area = getLocalFloatBounds();
        DBG(getLocalFloatBounds().getWidth());
        area.reduce(100, 100);
        path.addEllipse(area);

        DBG(m_valueTree.toXmlString());

        auto transform = AffineTransform
        (
            /**
                The resulting matrix is:

                (mat00 mat01 mat02)
                (mat10 mat11 mat12)
                (  0     0     1  )
            */
            /*00*/ static_cast<float>(m_valueTree.getPropertyAsValue(ParameterIDS[ParameterEnum::slider1], false).getValue()),
            /*01*/ static_cast<float>(m_valueTree.getPropertyAsValue(ParameterIDS[ParameterEnum::slider3], false).getValue()),
            /*02*/ static_cast<float>(m_valueTree.getPropertyAsValue(ParameterIDS[ParameterEnum::slider5], false).getValue()),
            /*10*/ static_cast<float>(m_valueTree.getPropertyAsValue(ParameterIDS[ParameterEnum::slider2], false).getValue()),
            /*11*/ static_cast<float>(m_valueTree.getPropertyAsValue(ParameterIDS[ParameterEnum::slider4], false).getValue()),
            /*12*/ static_cast<float>(m_valueTree.getPropertyAsValue(ParameterIDS[ParameterEnum::slider6], false).getValue())

        );

        path.applyTransform(transform);
        g.setColour(juce::Colours::aliceblue);
        g.fillPath(path);
    }
}

Rectangle<float> AffineTransformsDisplay::getLocalFloatBounds()
{
    return
    {
        static_cast<float>(getX()),
        static_cast<float>(getY()),
        static_cast<float>(getWidth()),
        static_cast<float>(getHeight())
    };
}

