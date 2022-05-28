#define _USE_MATH_DEFINES

#ifndef _LLDSP_LIB_
#define _LLDSP_LIB_

#include <cmath>
#include <math.h>

namespace lldsp
{


    struct RingBuffer;
    struct Chorus;
    class SignalGenerator;
    /**
     * @brief Applies tanh waveshaping to a signal on a per sample basis.
     * 
     * @param sample 
     * @param gain 
     * @return float 
     */
    static double TanhDistortion(float sample, float gain) 
    {
        return std::tanh(sample * gain) / gain;
    }
}

struct lldsp::RingBuffer
{
    int N;
    double* buffer;
    int start = 0;
    int end = 0;
    bool full = false;
    int length = 0;

    RingBuffer(int size) : N(size)
    {
        buffer = new double[size];
    }

    void Push(double item)
    {
        buffer[end++] = item;
        if (end == N) { full = true; }
        if (!full) { length++; }
        end %= N;
    }

    double Pop()
    {
        if (full)
        {
            double item = buffer[start++];
            start %= N;
            return item;
        }
        return 0;
    }

    double Get(int delaySamples)
    {
        if (full)
        {
            int delayIndex = end - delaySamples;
            if (delayIndex < 0) { delayIndex = N + delayIndex; }
            return buffer[delayIndex];
        }
        return 0;
    }
};

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
            value = level * sineValue;
            break;
        case Waveforms::Triangle:
            value += (sineValue >= 0) ? triangleDelta : -triangleDelta;
            break;
        case Waveforms::Square:
            value = (sineValue >= 0) ? 1 : -1;
            break;
        }

        return value;
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

static double mapRange(double input, double inStart, double inEnd, double outStart, double outEnd)
{
    double slope = (outEnd - outStart) / (inEnd - inStart);
    return outStart + slope * (input - inStart);
}

struct lldsp::Chorus
{
    lldsp::RingBuffer delay = lldsp::RingBuffer(48000);
    lldsp::SignalGenerator oscillator;
    double frequency = 100;

    Chorus()
    {
    }

    Chorus(double sampleRate)
    {
        delay = lldsp::RingBuffer(sampleRate);
        oscillator.SetSampleRate(sampleRate);
    }

    // Ring Buffer Wrappers
    void Push(double item) { delay.Push(item); }
    double Pop() { return delay.Pop(); }

    double Get()
    {
        double index = oscillator.OscCycleWithFreq(frequency, 1);
        index += 1;
        index = mapRange(index, 0, 2, 408, 616);

        return delay.Get(index);
    }

    void SetFrequency(double freq)
    {
        frequency = freq;
    }

    void SetWaveform(lldsp::SignalGenerator::Waveforms waveform)
    {
        oscillator.SetWaveform(waveform);
    }

};

#endif