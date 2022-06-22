#include "lldsp.effects.h"
#include "lldsp.utils.h"

static double mapRange(double input, double inStart, double inEnd, double outStart, double outEnd)
{
    double slope = (outEnd - outStart) / (inEnd - inStart);
    return outStart + slope * (input - inStart);
}

lldsp::effects::Chorus::Chorus()
{
}

lldsp::effects::Chorus::Chorus(double sampleRate)
{
    delay = lldsp::utils::RingBuffer(sampleRate);
    oscillator.SetSampleRate(sampleRate);
}

// Ring Buffer Wrappers
void lldsp::effects::Chorus::Push(double item) { delay.Push(item); }
double lldsp::effects::Chorus::Pop() { return delay.Pop(); }

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