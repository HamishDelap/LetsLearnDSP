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

class SmallKnob : public LookAndFeel_V3
{
public:
    SmallKnob() {}
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;
};

class TestAudioProcessorEditor : public AudioProcessorEditor, public Slider::Listener
{
public:
    TestAudioProcessorEditor (TestAudioProcessor&);
    ~TestAudioProcessorEditor() override;

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    //==============================================================================
    void sliderValueChanged(Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TestAudioProcessor& audioProcessor;

    Slider timeSlider;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> timeSliderAttachment;

    Slider feedbackSlider;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> feedbackSliderAttachment;

    SmallKnob knobLookAndFeel;

    GUILang m_guiLang;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestAudioProcessorEditor)
};
