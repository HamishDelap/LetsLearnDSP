#pragma once
#include "lldsp.oscillators.signalgenerator.h"
#include "lldsp.utils.h"

namespace lldsp::effects
{
    class Chorus
    {
    public:
        Chorus() {};
        Chorus(double sampleRate);
        void Push(double item);
        double Pop();
        double Get();
        void SetFrequency(double freq);
        void SetWaveform(lldsp::oscillators::Waveforms waveform);

    private:
        lldsp::utils::RingBuffer m_delay = lldsp::utils::RingBuffer(48000);
        lldsp::oscillators::SignalGenerator m_oscillator;
        double m_frequency = 100;
    };
}

