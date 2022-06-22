#include "lldsp.effects.h"
#include "lldsp.utils.h"

static double mapRange(double input, double inStart, double inEnd, double outStart, double outEnd)
{
    double slope = (outEnd - outStart) / (inEnd - inStart);
    return outStart + slope * (input - inStart);
}

// Chorus
lldsp::effects::Chorus::Chorus(double sampleRate)
{
    delay = lldsp::utils::RingBuffer(sampleRate);
    oscillator.SetSampleRate(sampleRate);
}

// Ring Buffer Wrappers
void lldsp::effects::Chorus::Push(double item) 
{
    delay.Push(item);
}

double lldsp::effects::Chorus::Pop() 
{
    return delay.Pop();
}

double lldsp::effects::Chorus::Get()
{
    double index = oscillator.OscCycleWithFreq(frequency, 1);
    index += 1;
    index = mapRange(index, 0, 2, 408, 616);

    return delay.Get(index);
}

void lldsp::effects::Chorus::SetFrequency(double freq)
{
    frequency = freq;
}

void lldsp::effects::Chorus::SetWaveform(lldsp::SignalGenerator::Waveforms waveform)
{
    oscillator.SetWaveform(waveform);
}


// Reverb
lldsp::effects::Reverb::Reverb(lldsp::effects::Reverb::Params parameters, double sampleRate)
{
    m_CombOne = lldsp::dsp::CombFilter(parameters.t1, parameters.g1, sampleRate);
    m_CombTwo = lldsp::dsp::CombFilter(parameters.t2, parameters.g2, sampleRate);
    m_CombThree = lldsp::dsp::CombFilter(parameters.t3, parameters.g3, sampleRate);
    m_CombFour = lldsp::dsp::CombFilter(parameters.t4, parameters.g4, sampleRate);

    m_AllPassOne = lldsp::dsp::AllPassFilter(parameters.t5, parameters.g5, sampleRate);
    m_AllPassTwo = lldsp::dsp::AllPassFilter(parameters.t6, parameters.g6, sampleRate);
}

double lldsp::effects::Reverb::Process(double sample)
{
    double combFiltersSummed = m_CombOne.Process(sample) + m_CombTwo.Process(sample) + m_CombThree.Process(sample) + m_CombFour.Process(sample);
    double firstAllPassResult = m_AllPassOne.Process(combFiltersSummed);
    double secondAllPassResult = m_AllPassTwo.Process(firstAllPassResult);

    return sample + secondAllPassResult * m_Parameters.g7;
}