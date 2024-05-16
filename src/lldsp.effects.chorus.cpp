#include "lldsp.oscillators.signalgenerator.h"
#include "lldsp.effects.chorus.h"
#include "lldsp.utils.h"


lldsp::effects::Chorus::Chorus(double sampleRate)
{
    m_delay = lldsp::utils::RingBuffer(sampleRate);
    m_oscillator.SetSampleRate(sampleRate);
}

// Ring Buffer Wrappers
void lldsp::effects::Chorus::Push(double item) 
{
    m_delay.Push(item);
}

double lldsp::effects::Chorus::Pop() 
{
    return m_delay.Pop();
}

double lldsp::effects::Chorus::Get()
{
    double index = m_oscillator.OscCycleWithFreq(m_frequency, 1);
    index += 1;
    index = lldsp::utils::mapRange(index, 0, 2, 408, 616);

    return m_delay.Get(index);
}

void lldsp::effects::Chorus::SetFrequency(double freq)
{
    m_frequency = freq;
}

void lldsp::effects::Chorus::SetWaveform(lldsp::oscillators::Waveforms waveform)
{
    m_oscillator.SetWaveform(waveform);
}
