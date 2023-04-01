#pragma once
#include <JuceHeader.h>
#include "../../Test/Source/PluginProcessor.h"

class DebugEditor : public juce::Component, public Timer
{
public:
    DebugEditor(TestAudioProcessor& audioProcessor);
    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

    void PaintWaveform(juce::Graphics& g, const int index);
    void PaintSpectrum(juce::Graphics& g, const int index);

private:
    ToggleButton pauseBtn;
    ToggleButton muteBtn;
    ArrowButton resetBtn;

    Label pauseBtnLabel;
    Label muteBtnLabel;
    Label resetBtnLabel;

    AffineTransform transform;
    TestAudioProcessor *m_audioProcessor;
};