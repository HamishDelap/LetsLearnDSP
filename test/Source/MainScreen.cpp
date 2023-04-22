/*
  ==============================================================================

    MainScreen.cpp
    Created: 1 Apr 2023 5:00:11pm
    Author:  hamis

  ==============================================================================
*/

#include "MainScreen.h"

#include <JuceHeader.h>
#include "MainScreen.h"
using namespace juce;

//==============================================================================
MainScreenComponent::MainScreenComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(372, 135);
    setFramesPerSecond(60);
}

MainScreenComponent::~MainScreenComponent()
{
}

void MainScreenComponent::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(getLookAndFeel().findColour(juce::Slider::thumbColourId));

    int radius = 70;

    juce::Point<float> p((float)getWidth() / 2.0f + 1.0f * (float)radius * std::sin((float)getFrameCounter() * 0.04f),
        (float)getHeight() / 2.0f + 1.0f * (float)radius * std::cos((float)getFrameCounter() * 0.04f));

    g.fillEllipse(p.x, p.y, 20.0f, 20.0f);
}

void MainScreenComponent::resized()
{
}

void MainScreenComponent::update()
{

}
