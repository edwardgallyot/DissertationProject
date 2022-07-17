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
    m_valueTree(treeToReferTo),
    m_serpinskiBounds(3),
    m_numChaosGamePoints(10000),
    m_serpinskiPoints(m_numChaosGamePoints)
{
    InitialiseSerpinkskiBounds();
    startTimer(33);
    
}

AffineTransformsDisplay::~AffineTransformsDisplay()
{
}

void AffineTransformsDisplay::valueChanged(Value& value)
{
}

void AffineTransformsDisplay::resized()
{
    if (!AssertResizeBounds())
    {
        m_screenData.resize(getWidth(), std::vector<Point<float>>(getHeight(), Point<float>(0.0f, 0.0f)));
    }
    InitialiseSerpinkskiBounds();
}

void AffineTransformsDisplay::timerCallback()
{
    repaint();
}

void AffineTransformsDisplay::StoreScreenData()
{
    if (!AssertResizeBounds())
    {
        m_screenData.resize(getWidth(), std::vector<Point<float>>(getHeight(), Point<float>(0.0f, 0.0f)));
    }
    for (auto y = 0; y < getHeight(); ++y)
    {
        for (auto x = 0; x < getWidth(); ++x)
        {
            m_screenData[x][y] = getLocalPoint(this, Point<float>(static_cast<float>(x), static_cast<float>(y)));
        }
    }
}

bool AffineTransformsDisplay::AssertResizeBounds()
{
    if (getWidth() != m_screenData.size())
    {
        return false;
    }
    
    if (m_screenData.size() > 0)
    {
        if (getHeight() != m_screenData[0].size())
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

void AffineTransformsDisplay::InitialiseSerpinkskiBounds()
{
    m_serpinskiBounds.clear();
    m_serpinskiBounds.push_back({ 0.0f, static_cast<float>(getHeight()) });
    m_serpinskiBounds.push_back({ static_cast<float>(getWidth()) / 2.0f, 0.0f});
    m_serpinskiBounds.push_back({ static_cast<float>(getWidth()), static_cast<float>(getHeight()) });
}

void AffineTransformsDisplay::ChaosGame()
{
    Random random;

    auto tmpPoint = Point<float> 
    (
        fmod(random.nextFloat(), static_cast<float>(getWidth())), 
        fmod(random.nextFloat(),static_cast<float>( getHeight()))
    );
    for (int i = 0; i < m_numChaosGamePoints; ++i) 
    {
        m_serpinskiPoints[i] = tmpPoint;
        auto randomInt = random.nextInt({ 0,3 });

        //DBG(randomInt);
        tmpPoint = 0.5f * (tmpPoint + m_serpinskiBounds[randomInt]);
        
    }
}

void AffineTransformsDisplay::paint(juce::Graphics& g)
{
    if (AssertResizeBounds())
    {
        StoreScreenData();
    }
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
        auto  area = GetLocalFloatBounds();
        area.reduce(100, 100);
        path.addEllipse(area);

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

    {
        g.setColour(juce::Colours::aqua);
        ChaosGame();

        for (auto i = 0; i < m_serpinskiPoints.size(); ++i)
        {
            g.fillRect(m_serpinskiPoints[i].getX(), m_serpinskiPoints[i].getY(), 2.0f, 2.0f);
        }
    }
}

Rectangle<float> AffineTransformsDisplay::GetLocalFloatBounds()
{
    return
    {
        static_cast<float>(getX()),
        static_cast<float>(getY()),
        static_cast<float>(getWidth()),
        static_cast<float>(getHeight())
    };
}

