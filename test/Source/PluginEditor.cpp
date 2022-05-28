/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <functional>
#include <filesystem>

void SmallKnob::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
                                 const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    Image knob = ImageCache::getFromMemory(BinaryData::customKnob1Reel_png, BinaryData::customKnob1Reel_pngSize);
    int frameCount = knob.getHeight() / knob.getWidth();
    int frameSize = knob.getWidth();

    const double div = slider.getMaximum() / frameCount;
    double pos = (int)(slider.getValue() / div);

    if (pos > 0) { pos = pos - 1; }
        
    g.drawImage(knob, x, y, width, height, 0, (int)(pos * frameSize), frameSize, frameSize, false);
}



//==============================================================================
TestAudioProcessorEditor::TestAudioProcessorEditor (TestAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    m_guiLang.Parse(std::filesystem::path("C:\\Users\\hamis\\Documents\\Dev\\AudioDevelopment\\LetsLearnDSP\\test\\Source\\test.gui"));
    m_guiLang.Convert();

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (750, 900);

    auto callback = [&](juce::Component& component) {
        addAndMakeVisible(component);
    };
    m_guiLang.Initialise(callback);

    LookAndFeel::setDefaultLookAndFeel(nullptr);

    Slider::TextEntryBoxPosition noDisplay = juce::Slider::TextEntryBoxPosition::NoTextBox;
    
    // Adding sliders
    addAndMakeVisible(timeSlider);
    timeSlider.setRange(-10.0, 10.0);
    timeSlider.setTextValueSuffix("");
    timeSlider.addListener(this);
    timeSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 100, 0));
    timeSlider.setSliderStyle(juce::Slider::Rotary);
    timeSlider.setTextBoxStyle(noDisplay, false, 1, 1);
    
    timeSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.stateManager.apvt, "TIME", timeSlider);

    //addAndMakeVisible(feedbackSlider);
    //feedbackSlider.setRange(-10.0, 10.0);
    //feedbackSlider.setTextValueSuffix("");
    //feedbackSlider.addListener(this);
    //feedbackSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 100, 0));
    //feedbackSlider.setSliderStyle(juce::Slider::Rotary);
    //feedbackSlider.setTextBoxStyle(noDisplay, false, 1, 1);
    //
    //feedbackSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.stateManager.apvt, "FEEDBACK", feedbackSlider);

    timeSlider.setLookAndFeel(&knobLookAndFeel);
    //feedbackSlider.setLookAndFeel(&knobLookAndFeel);
}

TestAudioProcessorEditor::~TestAudioProcessorEditor()
{
    timeSlider.setLookAndFeel(nullptr);
    feedbackSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void TestAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    g.drawImageAt(background, 0, 0);
}

void TestAudioProcessorEditor::resized()
{
    m_guiLang.SetBounds();
    timeSlider.setBounds(223, 300, 305, 305);
    //feedbackSlider.setBounds(50, 180, 70, 70);
}

void TestAudioProcessorEditor::sliderValueChanged(Slider* slider) {
    
}