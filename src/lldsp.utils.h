#pragma once
#include "lldsp.h"
#include <vector>

// Utils
namespace lldsp::utils
{
    static double mapRange(double input, double inStart, double inEnd, double outStart, double outEnd)
    {
        double slope = (outEnd - outStart) / (inEnd - inStart);
        return outStart + slope * (input - inStart);
    }

    static void HanningWindow(std::vector<double>& data)
    {
        int N = data.size();
        for (int n = 0; n < N; n++) {
            double w = 0.5 - 0.5 * cos(2 * M_PI * n / (N - 1));
            data[n] *= w;
        }
    }

    class RingBuffer
    {
    public:
        RingBuffer(int size);
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