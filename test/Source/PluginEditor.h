/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MainScreen.h"
#include "..\..\src\DbgEngine\DebugEditor.h"
#include <unordered_map>

//==============================================================================
/**
*/
using namespace juce;

class BigKnob : public LookAndFeel_V3
{
public:
    BigKnob() {}
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;
};

class SmallKnob : public LookAndFeel_V3
{
public:
    SmallKnob() {}
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;
};

static const std::unordered_map<std::string, std::array<std::string, 3>> FX_LABELS = {
    {"Chorus", {"-", "-", "-"}},
    {"Distortion", {"-", "-", "-"}},
    {"Reverb", {"-", "Length", "-"}},
    {"Delay", {"-", "-", "-"}},
    {"Granular", {"Reverb", "Time", "Size"}}
};

class TestAudioProcessorEditor : public AudioProcessorEditor, public Slider::Listener, public Button::Listener
{
public:
    TestAudioProcessorEditor (TestAudioProcessor&);
    ~TestAudioProcessorEditor() override;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    //==============================================================================
    void sliderValueChanged(Slider* slider) override;

    void buttonClicked(juce::Button* button) override;

private:
    void UpdateLabels();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TestAudioProcessor& audioProcessor;

    Slider bigKnob;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> bigKnobAttachment;

    Slider leftKnob;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> leftKnobAttachment;

    Slider centerKnob;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> centerKnobAttachment;
    
    Slider rightKnob;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> rightKnobAttachment;

    ArrowButton nextFxButton;
    ArrowButton prevFxButton;

    ToggleButton dbgBtn;

    SmallKnob smallKnobLookAndFeel;
    BigKnob bigKnobLookAndFeel;

    Label fxLabel;
    Label leftParamLabel;
    Label centerParamLabel;
    Label rightParamLabel;

    MainScreenComponent mainScreen;

    ScopedPointer<ResizableWindow> m_DebugWindow;
    juce::Component m_DebugEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestAudioProcessorEditor)
};
