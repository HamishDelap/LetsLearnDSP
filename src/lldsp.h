#define _USE_MATH_DEFINES

#ifndef _LLDSP_LIB_
#define _LLDSP_LIB_

#include <cmath>
#include <math.h>

namespace lldsp
{
    class SignalGenerator;
}

class lldsp::SignalGenerator
{
public:
    enum class Waveforms 
    {
        Sin,
        Triangle,
        Square
    };

    SignalGenerator() 
    {
        localSampleRate = 1;
        waveform = Waveforms::Sin;
        currentAngle = 1;
        value = 1;
    }

    SignalGenerator(double sampleRate, Waveforms wave)
    {
        localSampleRate = sampleRate;
        waveform = wave;
    }

    void FreqToAngle(double frequency) 
    {
        auto cyclesPerSecond = frequency;
        auto cyclesPerSample = cyclesPerSecond / localSampleRate;

        angleDelta = cyclesPerSample * 2.0 * M_PI;

        currentAngle += angleDelta;
    }

    double OscCycleWithFreq(double frequency, double level) 
    {
        FreqToAngle(frequency);
        sineValue = sin(currentAngle);
        switch (waveform) 
        {
        case Waveforms::Sin:
            value = sineValue;
            break;
        case Waveforms::Triangle:
            value += (sineValue >= 0) ? triangleDelta : -triangleDelta;
            break;
        case Waveforms::Square:
            value = (sineValue >= 0) ? 1 : -1;
            break;
        }

        return level * value;
    }

    double OscCycleWithAngle(double angle, double level) 
    {
        sineValue = sin(angle);
        switch (waveform) 
        {
        case Waveforms::Sin:
            value = level * sineValue;
            break;
        case Waveforms::Triangle:
            value += (sineValue >= 0) ? triangleDelta : -triangleDelta;
            value = level * value;
            break;
        case Waveforms::Square:
            value = (sineValue >= 0) ? 1 : -1;
            value *= level;
            break;
        }

        return value;
    }

    void SetWaveform(Waveforms waveshape)
    {
        waveform = waveshape;
    }

    void SetSampleRate(double samplerate) 
    {
        localSampleRate = samplerate;
    }

private:
    double currentAngle = {};
    double angleDelta = {};
    double localSampleRate = {};
    Waveforms waveform = Waveforms::Sin;
    double value = {};

    double sineValue = 0;
    double triangleDelta = 0.1;
};



#endif