#pragma once
#include "lldsp.h"

// Utils
namespace lldsp::utils
{
    class RingBuffer
    {
    public:
        RingBuffer(int size);
        void Push(double item);
        double Pop();
        double Get(int delaySamples);
        double Get(double delayTime, double samplerate);

    private:
        int N;
        double* buffer;
        int start = 0;
        int end = 0;
        bool full = false;
        int length = 0;
    };
}