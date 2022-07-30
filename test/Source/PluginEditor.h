/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <GUILang.hpp>

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
    void UpdateFxLabel();

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

    SmallKnob smallKnobLookAndFeel;
    BigKnob bigKnobLookAndFeel;

    Label fxLabel;

    GUILang m_guiLang;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestAudioProcessorEditor)
};
