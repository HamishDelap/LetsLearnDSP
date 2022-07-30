#pragma once
#include <JuceHeader.h>
#include "../Source/PluginProcessor.h"

class DebugEditor : public juce::Component, public Timer
{
public:
    DebugEditor(TestAudioProcessor& audioProcessor);
    void paint(juce::Graphics& g) override;
    void timerCallback() override;
private:
    int specHeight;
    int specWidth;
    AffineTransform transform;
    TestAudioProcessor *m_AudioProcessor;
};