#include "lldsp.utils.h"
#include <math.h>

lldsp::utils::RingBuffer::RingBuffer(int size) : N(size)
{
    buffer = new double[size];
}

void lldsp::utils::RingBuffer::Push(double item)
{
    buffer[end++] = item;
    if (end == N) { full = true; }
    if (!full) { length++; }
    end %= N;
}

double lldsp::utils::RingBuffer::Pop()
{
    if (full)
    {
        double item = buffer[start++];
        start %= N;
        return item;
    }
    return 0;
}

// Get value using samples
double lldsp::utils::RingBuffer::Get(int delaySamples)
{
    if (full)
    {
        int delayIndex = end - delaySamples;
        if (delayIndex < 0) { delayIndex = N + delayIndex; }
        return buffer[delayIndex];
    }
    return 0;
}

// Get value using time in s
double lldsp::utils::RingBuffer::Get(double delayTime, double samplerate)
{
    int delaySamples = samplerate / delayTime;
    return Get(delaySamples);
}

double* lldsp::utils::RingBuffer::GetBuffer()
{
    return buffer;
}

double lldsp::utils::RingBuffer::GetRMS()
{
    double squareTotal = 0;
    for (int i = 0; i <= N; i++)
    {
        squareTotal += buffer[i] * buffer[i];
    }

    if (!full)
    {
        return -1;
    }
    return std::pow(squareTotal / N, 0.5);
}