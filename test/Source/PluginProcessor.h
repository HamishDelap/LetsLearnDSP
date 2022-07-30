/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "..\..\src\lldsp.h"
#include "..\..\src\lldsp.utils.h"
#include "..\..\src\lldsp.effects.h"
#include "..\..\src\JStateManager.h"
#include "..\..\src\SignalGenerator.h"
#include "..\..\src\DbgEngine\DebugProcessor.h"

//==============================================================================
/**
*/
class TestAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    TestAudioProcessor();
    ~TestAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    JStateManager stateManager;

    enum class Effect {
        Delay,
        Distortion,
        Chorus,
        Reverb
    };
    Effect m_Effects[4] = { Effect::Delay, Effect::Distortion, Effect::Chorus, Effect::Reverb };
    int m_CurrentEffect = 0;

    DebugProcessor m_DebugProcessor;

private:
    lldsp::effects::Chorus chorus;
    lldsp::utils::RingBuffer delay;
    lldsp::effects::Reverb reverb;

    double m_Samplerate = 44100;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestAudioProcessor)
};
