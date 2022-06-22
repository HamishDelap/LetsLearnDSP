#pragma once
#include "lldsp.h"
#include "lldsp.utils.h"
#include "lldsp.dsp.h"

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
        Chorus() {};
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

    class Reverb
    {
    public:
        struct Params
        {
            // Comb Filter Delays
            double t1;
            double t2;
            double t3;
            double t4;

            // All Pass Filter Delays
            double t5;
            double t6;

            // Comb Filter Gains
            double g1;
            double g2;
            double g3;
            double g4;

            // All Pass Filter Gains
            double g5;
            double g6;

            // Final Gain
            double g7;
        };

        Reverb() {};
        Reverb(Params parameters, double sampleRate);

        double Process(double sample);
    private:
        Params m_Parameters = {};

        lldsp::dsp::CombFilter m_CombOne;
        lldsp::dsp::CombFilter m_CombTwo;
        lldsp::dsp::CombFilter m_CombThree;
        lldsp::dsp::CombFilter m_CombFour;

        lldsp::dsp::AllPassFilter m_AllPassOne;
        lldsp::dsp::AllPassFilter m_AllPassTwo;
    };

}