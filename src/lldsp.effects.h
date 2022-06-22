#pragma once
#include "lldsp.h"
#include "lldsp.utils.h"

// Effects
namespace lldsp::effects
{
    static double TanhDistortion(float sample, float gain)
    {
        return std::tanh(sample * gain) / gain;
    }


    class Chorus
    {
    public:
        Chorus();
        Chorus(double sampleRate);
        void Push(double item);
        double Pop();
        double Get();
        void SetFrequency(double freq);
        void SetWaveform(lldsp::SignalGenerator::Waveforms waveform);

    private:
        lldsp::utils::RingBuffer delay = lldsp::utils::RingBuffer(48000);
        lldsp::SignalGenerator oscillator;
        double frequency = 100;
    };
    class Reverb;

}