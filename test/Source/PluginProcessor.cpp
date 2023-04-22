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
                       ), m_stateManager(*this), m_chorus(), m_delay(44100)
#endif
{
    m_debugProcessor;
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
    m_samplerate = sampleRate;
    m_chorus = lldsp::effects::Chorus(sampleRate);
    m_chorus.SetFrequency(0.25);

    double reverbTime = 0.1;
    // This needs to be turned into member function of reverb

    m_reverb = lldsp::effects::Reverb(sampleRate);
    m_delay = lldsp::utils::RingBuffer(sampleRate);

    m_granularDelayLine.SetGrainCount(30);
    m_granularDelayLine.SetGrainSize(30, sampleRate);
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
    
void TestAudioProcessor::reset()
{
    m_resetFlag = true;
}

void TestAudioProcessor::mute()
{
    m_muteFlag = true;
}

void TestAudioProcessor::unmute()
{
    m_muteFlag = false;
}

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

        if (m_effects[m_currentEffect] == Effect::Chorus)
        {
            float freq = static_cast<float>(m_stateManager.apvt.getRawParameterValue("BIGKNOB")->load());

            m_chorus.SetFrequency(freq);

            double delayVal = m_chorus.Get();
            channelData[sample] = delayVal + channelData[sample];

            m_chorus.Push(channelData[sample] * 0.75);
            leftChannelData[sample] = channelData[sample];
        }
        else if (m_effects[m_currentEffect] == Effect::Distortion)
        {
            double gain = static_cast<double>(m_stateManager.apvt.getRawParameterValue("BIGKNOB")->load());

            //channelData[sample] = lldsp::effects::TanhDistortion(channelData[sample], gain);
            channelData[sample] = lldsp::effects::Overdrive(channelData[sample], gain);
            leftChannelData[sample] = channelData[sample];
        }
        else if (m_effects[m_currentEffect] == Effect::Delay)
        {
            float delayTime = static_cast<float>(m_stateManager.apvt.getRawParameterValue("BIGKNOB")->load());

            delayTime = m_samplerate * (delayTime / 10);

            double delayVal = m_delay.Get(delayTime);
            channelData[sample] = delayVal + channelData[sample];

            m_delay.Push(channelData[sample] * 0.75);
            leftChannelData[sample] = channelData[sample];
        }
        else if (m_effects[m_currentEffect] == Effect::Reverb)
        {
            float drywet = static_cast<float>(m_stateManager.apvt.getRawParameterValue("BIGKNOB")->load());
            float time = static_cast<float>(m_stateManager.apvt.getRawParameterValue("CENTERKNOB")->load());
            channelData[sample] = m_reverb.Process(channelData[sample], time, drywet / 10);
            leftChannelData[sample] = channelData[sample];
        }
        else if (m_effects[m_currentEffect] == Effect::Granular)
        {
            try
            {
                float grainSize = static_cast<float>(m_stateManager.apvt.getRawParameterValue("RIGHTKNOB")->load());
                float graindrywet = static_cast<float>(m_stateManager.apvt.getRawParameterValue("BIGKNOB")->load());
                float reverbdrywet = static_cast<float>(m_stateManager.apvt.getRawParameterValue("LEFTKNOB")->load());
                float time = static_cast<float>(m_stateManager.apvt.getRawParameterValue("CENTERKNOB")->load());
                m_granularDelayLine.SetGrainSize(jmap(grainSize, 0.0f, 10.0f, 10.0f, 100.0f), m_samplerate);
                channelData[sample] = (channelData[sample] * (1.0 - (graindrywet / 10))) + (m_granularDelayLine.Process(channelData[sample]) * (graindrywet / 10));
                channelData[sample] = m_reverb.Process(channelData[sample], time, reverbdrywet / 10);
                leftChannelData[sample] = channelData[sample];
            }
            catch (std::exception ex)
            {
                channelData[sample] = 0;
            }
        }

        if (m_resetFlag || m_muteFlag)
        {
            channelData[sample] = 0;
            leftChannelData[sample] = 0;
        }

        m_debugProcessor.PushNextSampleIntoFifo(channelData[sample]);
    }
    
    if (m_resetFlag)
    {
        // Reset Effects
        m_chorus = lldsp::effects::Chorus(m_samplerate);
        m_reverb = lldsp::effects::Reverb(m_samplerate);
        m_delay = lldsp::utils::RingBuffer(m_samplerate);
        //m_granularDelayLine = lldsp::effects::GranularDelayLine();
        m_resetFlag = false;
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
