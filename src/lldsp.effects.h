#pragma once
#include "lldsp.h"
#include "lldsp.utils.h"
#include "lldsp.dsp.h"
#include "lldsp.effects.granular.h"

// Effects
namespace lldsp::effects
{
    static double TanhWaveshaper(double sample, double gain)
    {
        return std::tanh(sample * gain) / gain;
    }

    static double DafxDistortion(double sample, double gain)
    {
        sample *= gain;
        double signPreservation = sample / std::abs(sample);
        return signPreservation * (1 - std::exp(((sample * sample)) / std::abs(sample)));
    }

    static double Overdrive(double sample, double gain)
    {
        sample *= gain;
        double absolute = std::abs(sample);

        if (absolute < 0.333333333333)
        {
            return sample * 2;
        }
        else if (absolute < 0.6666666666)
        {
            return (3 - ((2 - 3 * sample) * (2 - 3 * sample))) / 3;
        }
        return sample;
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
        lldsp::utils::RingBuffer m_delay = lldsp::utils::RingBuffer(48000);
        lldsp::SignalGenerator m_oscillator;
        double m_frequency = 100;
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
        Reverb(double sampleRate);

        double Process(double sample, double time, double amount);
    private:
        void UpdateParameters(double time, double amount);

        Params m_parameters = {
            // Time
            0.03,
            0.035,
            0.04,
            0.045,
            0.005,
            0.0017,
            // Gain
            -0.9332543,
            -0.9225714,
            -0.9120108,
            -0.9015711,
            0.7,
            0.7,
            0.5
        };

        lldsp::dsp::CombFilter m_combOne;
        lldsp::dsp::CombFilter m_combTwo;
        lldsp::dsp::CombFilter m_combThree;
        lldsp::dsp::CombFilter m_combFour;

        lldsp::dsp::AllPassFilter m_allPassOne;
        lldsp::dsp::AllPassFilter m_allPassTwo;
    };
}