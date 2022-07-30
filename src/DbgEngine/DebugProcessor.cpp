#include "DebugProcessor.h"

DebugProcessor::DebugProcessor() :fft(fftOrder), windowFunction(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
    DBG("Creating Spectrum Processor...");
    fifoIndex = 0;
}

void DebugProcessor::PushNextSampleIntoFifo(float sample)
{
    if (fifoIndex == fftSize)
    {
        if (!nextFFTBlockReady)
        {
            juce::zeromem(fftData, sizeof(fftData));
            memcpy(fftData, fifo, sizeof(fifo));
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[fifoIndex++] = sample;
}

void DebugProcessor::CalcNextFrameOfSpectrum()
{
    // Apply the windowing funtion
    windowFunction.multiplyWithWindowingTable(fftData, fftSize);

    // Apply the forwardFFT
    fft.performFrequencyOnlyForwardTransform(fftData);

    auto minLevel = -100.0f;
    auto maxLevel = 0.0f;

    for (int i = 0; i < scopeSize; i++)
    {
        auto skewedXAxis = 1.0f - std::exp(std::log(1.0f - (float)i / (float)scopeSize) * 0.2f);
        auto fftDataIndex = juce::jlimit(0, fftSize / 2, (int)(skewedXAxis * (float)fftSize * 0.5f));
        auto currentLevel = juce::jmap(juce::jlimit(minLevel, maxLevel, juce::Decibels::gainToDecibels(fftData[fftDataIndex]) -
            juce::Decibels::gainToDecibels((float)fftSize)), minLevel, maxLevel, 0.0f, 1.0f);
        scopeData[i] = currentLevel;
    }
}