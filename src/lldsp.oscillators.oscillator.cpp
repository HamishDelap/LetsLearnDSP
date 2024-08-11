#include "lldsp.oscillators.oscillator.h"

namespace lldsp::oscillators
{
	static double TWO_PI = 2 * M_PI;

	void Oscillator::SetWaveForm(const Waveform& waveform)
	{
		m_waveform = waveform;
	}
	
	void Oscillator::SetSampleRate(const double sampleRate)
	{
		m_samplerate = sampleRate;
	}

	double Oscillator::Sample(const double frequency, const double level)
	{
		m_phase += TWO_PI * frequency / m_samplerate;

		// Wrap phase to the range [0, 2PI]
		m_phase = fmod(m_phase, TWO_PI);

		double output = 0;
		switch (m_waveform)
		{
		case Waveform::Sin:
		{
			output = level * sin(m_phase);
			break;
		}
		case Waveform::Square:
		{
			output = level * ((sin(m_phase)) >= 0.0 ? 1.0 : -1);
			break;
		}
		case Waveform::Triangle:
		{
			// Triangle wave
			double value = 2.0 * (m_phase / (TWO_PI));
			if (value < 1.0)
			{
				output = level * (2.0 * value - 1.0);  // Rising edge
			}
			else
			{
				output = level * (3.0 - 2.0 * value);  // Falling edge
			}
			break;
		}
		case Waveform::Sawtooth:
		{
			output = level * (2.0 * (m_phase / (TWO_PI)) - 1.0);
			break;
		}
		}

		return output;
	}

	void Oscillator::Reset()
	{
		m_phase = 0;
	}
}

