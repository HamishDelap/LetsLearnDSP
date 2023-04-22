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

    void reset();
    void mute();
    void unmute();

    JStateManager m_stateManager;

    enum class Effect {
        Delay,
        Distortion,
        Chorus,
        Reverb,
        Granular
    };
    Effect m_effects[5] = { Effect::Delay, Effect::Distortion, Effect::Chorus, Effect::Reverb, Effect::Granular };
    int m_currentEffect = 0;

    DebugProcessor m_debugProcessor;

private:
    bool m_resetFlag = false;
    bool m_muteFlag = false;

    lldsp::effects::Chorus m_chorus;
    lldsp::utils::RingBuffer m_delay;
    lldsp::effects::Reverb m_reverb;
    lldsp::effects::GranularDelayLine m_granularDelayLine;
    std::vector<double> m_granularBuffer;
    int m_currentGrain = 0;
    int m_currentGrainSample = 0;

    double m_samplerate = 44100;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestAudioProcessor)
};
