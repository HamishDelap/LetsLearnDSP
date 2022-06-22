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
    Image knob = ImageCache::getFromMemory(BinaryData::customKnob2Reel_png, BinaryData::customKnob2Reel_pngSize);
    int frameCount = knob.getHeight() / knob.getWidth();
    int frameSize = knob.getWidth();

    const double div = slider.getMaximum() / frameCount;
    double pos = (int)(slider.getValue() / div);

    if (pos > 0) { pos = pos - 1; }
        
    g.drawImage(knob, x, y, width, height, 0, (int)(pos * frameSize), frameSize, frameSize, false);
}


//==============================================================================
TestAudioProcessorEditor::TestAudioProcessorEditor (TestAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), nextFxButton("nextFx", 0.0, Colour(190, 190, 190)), prevFxButton("prevFx", 0.5, Colour(190, 190, 190))
{
    m_guiLang.Parse(std::filesystem::path("C:\\Users\\hamis\\Documents\\Dev\\AudioDevelopment\\LetsLearnDSP\\test\\Source\\test.gui"));
    m_guiLang.Convert();

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (750, 900);

    auto callback = [&](juce::Component& component) {
        //addAndMakeVisible(component);
    };
    m_guiLang.Initialise(callback);

    LookAndFeel::setDefaultLookAndFeel(nullptr);

    Slider::TextEntryBoxPosition noDisplay = juce::Slider::TextEntryBoxPosition::NoTextBox;

    addAndMakeVisible(nextFxButton);
    nextFxButton.addListener(this);
    addAndMakeVisible(prevFxButton);
    prevFxButton.addListener(this);
    
    // Adding sliders
    addAndMakeVisible(timeSlider);
    timeSlider.setRange(-10.0, 10.0);
    timeSlider.setTextValueSuffix("");
    timeSlider.addListener(this);
    timeSlider.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 100, 0));
    timeSlider.setSliderStyle(juce::Slider::Rotary);
    timeSlider.setTextBoxStyle(noDisplay, false, 1, 1);
    
    timeSliderAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.stateManager.apvt, "FREQ", timeSlider);

    addAndMakeVisible(fxLabel);
    fxLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    fxLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
    UpdateFxLabel();

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
    timeSlider.setBounds(223, 298, 305, 305);

    nextFxButton.setBounds(getWidth() - 200 - 30, 145, 30, 30);
    prevFxButton.setBounds(200, 145, 30, 30);

    fxLabel.setBounds(250, 120, 100, 80);
    //feedbackSlider.setBounds(50, 180, 70, 70);
}

void TestAudioProcessorEditor::sliderValueChanged(Slider* slider) 
{
    
}

void TestAudioProcessorEditor::UpdateFxLabel()
{
    if (audioProcessor.m_Effects[audioProcessor.m_CurrentEffect] == TestAudioProcessor::Effect::Chorus)
    {
        fxLabel.setText("Chorus", juce::NotificationType::dontSendNotification);
    }
    else if (audioProcessor.m_Effects[audioProcessor.m_CurrentEffect] == TestAudioProcessor::Effect::Delay)
    {
        fxLabel.setText("Delay", juce::NotificationType::dontSendNotification);
    }
    else if (audioProcessor.m_Effects[audioProcessor.m_CurrentEffect] == TestAudioProcessor::Effect::Distortion)
    {
        fxLabel.setText("Distortion", juce::NotificationType::dontSendNotification);
    }
}

void TestAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &nextFxButton)
    {
        audioProcessor.m_CurrentEffect += 1;
        if (audioProcessor.m_CurrentEffect > 2) { audioProcessor.m_CurrentEffect = 0; }
        UpdateFxLabel();
    }
    else if (button == &prevFxButton)
    {
        audioProcessor.m_CurrentEffect -= 1;
        if (audioProcessor.m_CurrentEffect < 0) { audioProcessor.m_CurrentEffect = 2; }
        UpdateFxLabel();
    }
}