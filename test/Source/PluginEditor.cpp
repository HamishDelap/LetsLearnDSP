/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <functional>
#include <filesystem>

void BigKnob::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
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

void SmallKnob::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    Image knob = ImageCache::getFromMemory(BinaryData::smallKnobReel_png, BinaryData::smallKnobReel_pngSize);
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

    addAndMakeVisible(fxLabel);
    fxLabel.setFont(juce::Font(24.0f, juce::Font::bold));
    fxLabel.setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
    UpdateFxLabel();

    // Adding sliders
    addAndMakeVisible(bigKnob);
    bigKnob.setRange(-10.0, 10.0);
    bigKnob.setTextValueSuffix("");
    bigKnob.addListener(this);
    bigKnob.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 100, 0));
    bigKnob.setSliderStyle(juce::Slider::Rotary);
    bigKnob.setTextBoxStyle(noDisplay, false, 1, 1);
    bigKnobAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.stateManager.apvt, "BIGKNOB", bigKnob);

    addAndMakeVisible(leftKnob);
    leftKnob.setRange(-10.0, 10.0);
    leftKnob.setTextValueSuffix("");
    leftKnob.addListener(this);
    leftKnob.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 100, 0));
    leftKnob.setSliderStyle(juce::Slider::Rotary);
    leftKnob.setTextBoxStyle(noDisplay, false, 1, 1);
    leftKnobAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.stateManager.apvt, "LEFTKNOB", leftKnob);

    addAndMakeVisible(centerKnob);
    centerKnob.setRange(-10.0, 10.0);
    centerKnob.setTextValueSuffix("");
    centerKnob.addListener(this);
    centerKnob.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 100, 0));
    centerKnob.setSliderStyle(juce::Slider::Rotary);
    centerKnob.setTextBoxStyle(noDisplay, false, 1, 1);
    centerKnobAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.stateManager.apvt, "CENTERKNOB", centerKnob);

    addAndMakeVisible(rightKnob);
    rightKnob.setRange(-10.0, 10.0);
    rightKnob.setTextValueSuffix("");
    rightKnob.addListener(this);
    rightKnob.setColour(Slider::ColourIds::backgroundColourId, Colour(0, 100, 0));
    rightKnob.setSliderStyle(juce::Slider::Rotary);
    rightKnob.setTextBoxStyle(noDisplay, false, 1, 1);
    rightKnobAttachment = new AudioProcessorValueTreeState::SliderAttachment(audioProcessor.stateManager.apvt, "RIGHTKNOB", rightKnob);

    bigKnob.setLookAndFeel(&bigKnobLookAndFeel);
    leftKnob.setLookAndFeel(&smallKnobLookAndFeel);
    centerKnob.setLookAndFeel(&smallKnobLookAndFeel);
    rightKnob.setLookAndFeel(&smallKnobLookAndFeel);

    m_DebugWindow = new ResizableWindow("New Window", true);
    m_DebugWindow->setUsingNativeTitleBar(true);
    m_DebugWindow->setCentrePosition(400, 400);
    m_DebugWindow->setVisible(false);
    m_DebugWindow->setResizable(false, false);
    m_DebugWindow->setContentOwned(new DebugEditor(audioProcessor), true); // maybe rename that to NewGUI or similar

    addAndMakeVisible(dbgBtn);
    dbgBtn.onClick = [this] {dbgBtn.getToggleState() ? m_DebugWindow->setVisible(true) : m_DebugWindow->setVisible(false); };
}

TestAudioProcessorEditor::~TestAudioProcessorEditor()
{
    bigKnob.setLookAndFeel(nullptr);
    leftKnob.setLookAndFeel(nullptr);
    centerKnob.setLookAndFeel(nullptr);
    rightKnob.setLookAndFeel(nullptr);
}

//==============================================================================
void TestAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::background2_png, BinaryData::background2_pngSize);
    g.drawImageAt(background, 0, 0);
}

void TestAudioProcessorEditor::resized()
{
    m_guiLang.SetBounds();
    bigKnob.setBounds(223, 298, 305, 305);

    nextFxButton.setBounds(getWidth() - 200 - 30, 145, 30, 30);
    prevFxButton.setBounds(200, 145, 30, 30);
    fxLabel.setBounds(250, 120, 100, 80);

    leftKnob.setBounds(115, 735, 120, 120);
    centerKnob.setBounds(314, 735, 120, 120);
    rightKnob.setBounds(532, 735, 120, 120);

    dbgBtn.setBounds(10, 10, 20, 20);
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
    else if (audioProcessor.m_Effects[audioProcessor.m_CurrentEffect] == TestAudioProcessor::Effect::Reverb)
    {
        fxLabel.setText("Reverb", juce::NotificationType::dontSendNotification);
    }
}

void TestAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &nextFxButton)
    {
        audioProcessor.m_CurrentEffect += 1;
        if (audioProcessor.m_CurrentEffect > 3) { audioProcessor.m_CurrentEffect = 0; }
        UpdateFxLabel();
    }
    else if (button == &prevFxButton)
    {
        audioProcessor.m_CurrentEffect -= 1;
        if (audioProcessor.m_CurrentEffect < 0) { audioProcessor.m_CurrentEffect = 3; }
        UpdateFxLabel();
    }
}