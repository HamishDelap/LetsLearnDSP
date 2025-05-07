#pragma once
#include "lldsp.math.h"
#include <vector>

// Utils
namespace lldsp::utils
{
    [[maybe_unused]] static double Interpolate(double distance, double timeInSeconds, double sampleRate)
    {
        return timeInSeconds > 0.0f ? (float)(distance / (timeInSeconds * sampleRate)) : -1.0f;
    }

    [[maybe_unused]] static double MapRange(double input, double inStart, double inEnd, double outStart, double outEnd)
    {
        double slope = (outEnd - outStart) / (inEnd - inStart);
        return outStart + slope * (input - inStart);
    }

    [[maybe_unused]] static void HanningWindow(std::vector<double>& data)
    {
        int N = static_cast<int>(data.size());
        for (int n = 0; n < N; n++) {
            double w = 0.5 - 0.5 * cos(2 * M_PI * n / (N - 1));
            data[n] *= w;
        }
    }

    class RingBuffer
    {
    public:
        RingBuffer(int size);
        RingBuffer(double size);
        void Push(double item);
        double Pop();
        double Get(int delaySamples);
        double Get(double delayTime, double samplerate);
        double* GetBuffer();

        double GetRMS();

    private:
        int N;
        double* buffer;
        int start = 0;
        int end = 0;
        bool full = false;
        int length = 0;
    };
}