/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

//==============================================================================
TestAudioProcessor::TestAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), stateManager(*this), chorus(), delay(44100)
#endif
{
}

TestAudioProcessor::~TestAudioProcessor()
{
}

//==============================================================================
const juce::String TestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TestAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TestAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TestAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TestAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TestAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TestAudioProcessor::getProgramName (int index)
{
    return {};
}

void TestAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    m_Samplerate = sampleRate;
    chorus = lldsp::effects::Chorus(sampleRate);
    chorus.SetFrequency(0.25);

    double reverbTime = 0.1;
    // This needs to be turned into member function of reverb

    reverb = lldsp::effects::Reverb(sampleRate);

    delay = lldsp::utils::RingBuffer(sampleRate);
}

void TestAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TestAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    int bufferNumSamples = buffer.getNumSamples();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    for (auto sample = 0; sample < bufferNumSamples; ++sample)
    {
        auto* channelData = buffer.getWritePointer(1, 0);
        auto* leftChannelData = buffer.getWritePointer(0, 0);

        if (m_Effects[m_CurrentEffect] == Effect::Chorus)
        {
            float freq = static_cast<float>(stateManager.apvt.getRawParameterValue("FREQ")->load());

            chorus.SetFrequency(freq);

            double delayVal = chorus.Get();
            channelData[sample] = delayVal + channelData[sample];

            chorus.Push(channelData[sample] * 0.75);
            leftChannelData[sample] = channelData[sample];
        }
        else if (m_Effects[m_CurrentEffect] == Effect::Distortion)
        {
            float gain = static_cast<float>(stateManager.apvt.getRawParameterValue("FREQ")->load());

            channelData[sample] = lldsp::effects::TanhDistortion(channelData[sample], gain);
            leftChannelData[sample] = channelData[sample];
        }
        else if (m_Effects[m_CurrentEffect] == Effect::Delay)
        {
            float delayTime = static_cast<float>(stateManager.apvt.getRawParameterValue("FREQ")->load());

            delayTime = m_Samplerate * (delayTime / 10);

            double delayVal = delay.Get(delayTime);
            channelData[sample] = delayVal + channelData[sample];

            delay.Push(channelData[sample] * 0.75);
            leftChannelData[sample] = channelData[sample];
        }
        else if (m_Effects[m_CurrentEffect] == Effect::Reverb)
        {
            float drywet = static_cast<float>(stateManager.apvt.getRawParameterValue("FREQ")->load());
            channelData[sample] =reverb.Process(channelData[sample], 2, drywet / 10);
            leftChannelData[sample] = channelData[sample];
        }
    }
}

//==============================================================================
bool TestAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TestAudioProcessor::createEditor()
{
    return new TestAudioProcessorEditor(*this);
}

//==============================================================================
void TestAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TestAudioProcessor();
}
