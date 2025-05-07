#pragma once
#include "lldsp.oscillators.waveforms.h"

namespace lldsp::oscillators
{
    class SignalGenerator
    {
    public:
        SignalGenerator();
        SignalGenerator(double sampleRate, Waveform wave);

        void FreqToAngle(double frequency);

        double OscCycleWithFreq(double frequency, double level);
        double OscCycleWithAngle(double angle, double level);

        void SetWaveform(Waveform waveshape);
        void SetSampleRate(double samplerate);

    private:
        double currentAngle = {};
        double angleDelta = {};
        double localSampleRate = {};
        Waveform waveform = Waveform::Sin;
        double value = {};

        double sineValue = 0;
        double triangleDelta = 0.1;
    };
}
