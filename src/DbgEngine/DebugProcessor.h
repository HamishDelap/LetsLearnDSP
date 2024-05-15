
#pragma once
#include <JuceHeader.h>
#include "../lldsp.utils.h"

namespace lldsp::debug 
{
    class DebugProcessor
    {
    public:
        DebugProcessor();
        void PushNextSampleIntoFifo(float sample);

        void CalcNextDebugFrame();

        void Pause();
        void Resume();

        enum
        {
            fftOrder = 11,
            fftSize = 1 << fftOrder,
            scopeSize = 512
        };

        float spectrumScopeData[scopeSize] = {};
        float waveformScopeData[scopeSize] = {};

        bool nextFFTBlockReady = false;

    private:
        void CalcNextFrameOfSpectrum();
        void CalcNextFrameOfWaveform();
        
        bool paused = false;
        
        juce::dsp::FFT fft;
        juce::dsp::WindowingFunction<float> windowFunction;

        float fifo[fftSize] = {};
        float fftData[2 * fftSize] = {};
        float waveformData[2 * fftSize] = {};
        int fifoIndex = 0;
    };
}
