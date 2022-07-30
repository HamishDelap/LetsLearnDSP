
#pragma once
#include <JuceHeader.h>
#include "../lldsp.utils.h"

class DebugProcessor
{
public:
	DebugProcessor();
    void PushNextSampleIntoFifo(float sample);
    void CalcNextFrameOfSpectrum();

    enum
    {
        fftOrder = 11,
        fftSize = 1 << fftOrder,
        scopeSize = 512
    };

    float scopeData[scopeSize] = {};
    bool nextFFTBlockReady = false;

private:

    juce::dsp::FFT fft;
    juce::dsp::WindowingFunction<float> windowFunction;

    float fifo[fftSize] = {};
    float fftData[2 * fftSize] = {};
    int fifoIndex = 0;
};