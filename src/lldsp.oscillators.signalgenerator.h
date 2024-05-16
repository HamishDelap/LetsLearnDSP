#pragma once
#include "lldsp.h"

namespace lldsp::oscillators
{
	enum class Waveforms
	{
		Sin,
		Triangle,
		Square
	};

    class SignalGenerator
    {
    public:
        SignalGenerator();
        SignalGenerator(double sampleRate, Waveforms wave);

        void FreqToAngle(double frequency);

        double OscCycleWithFreq(double frequency, double level);
        double OscCycleWithAngle(double angle, double level);

        void SetWaveform(Waveforms waveshape);
        void SetSampleRate(double samplerate);

    private:
        double currentAngle = {};
        double angleDelta = {};
        double localSampleRate = {};
        Waveforms waveform = Waveforms::Sin;
        double value = {};

        double sineValue = 0;
        double triangleDelta = 0.1;
    };
}
