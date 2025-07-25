#include "lldsp.oscillators.signalgenerator.h"
#include "lldsp.math.h"

namespace lldsp::oscillators
{
	SignalGenerator::SignalGenerator()
	{
		localSampleRate = 1;
		waveform = Waveform::Sin;
		currentAngle = 1;
		value = 1;
	}

	SignalGenerator::SignalGenerator(double sampleRate, Waveform wave)
	{
		localSampleRate = sampleRate;
		waveform = wave;
	}

	void SignalGenerator::FreqToAngle(double frequency)
	{
		auto cyclesPerSecond = frequency;
		auto cyclesPerSample = cyclesPerSecond / localSampleRate;

		angleDelta = cyclesPerSample * 2.0 * M_PI;

		currentAngle += angleDelta;
	}

	double SignalGenerator::OscCycleWithFreq(double frequency, double level)
	{
		FreqToAngle(frequency);
		sineValue = sin(currentAngle);
		switch (waveform)
		{
		case Waveform::Sin:
			value = sineValue;
			break;
		case Waveform::Triangle:
			value += (sineValue >= 0) ? triangleDelta : -triangleDelta;
			break;
		case Waveform::Square:
			value = (sineValue >= 0) ? 1 : -1;
			break;
		}

		return level * value;
	}

	double SignalGenerator::OscCycleWithAngle(double angle, double level)
	{
		sineValue = sin(angle);
		switch (waveform)
		{
		case Waveform::Sin:
			value = level * sineValue;
			break;
		case Waveform::Triangle:
			value += (sineValue >= 0) ? triangleDelta : -triangleDelta;
			value = level * value;
			break;
		case Waveform::Square:
			value = (sineValue >= 0) ? 1 : -1;
			value *= level;
			break;
		}

		return value;
	}

	void SignalGenerator::SetWaveform(Waveform waveshape)
	{
		waveform = waveshape;
	}

	void SignalGenerator::SetSampleRate(double samplerate)
	{
		localSampleRate = samplerate;
	}

}
