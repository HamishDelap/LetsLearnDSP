/*
  ==============================================================================

    MainScreen.h
    Created: 1 Apr 2023 5:00:11pm
    Author:  hamis

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class MainScreenComponent : public juce::AnimatedAppComponent
{
public:
    MainScreenComponent();
    ~MainScreenComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void update() override;

private:

    juce::Rectangle<int> m_rectangle;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainScreenComponent)
};


