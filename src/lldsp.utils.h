#pragma once
#include "lldsp.h"

// Utils
namespace lldsp::utils
{
    static double mapRange(double input, double inStart, double inEnd, double outStart, double outEnd)
    {
        double slope = (outEnd - outStart) / (inEnd - inStart);
        return outStart + slope * (input - inStart);
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