#include "lldsp.oscillators.oscillator.h"

namespace lldsp::oscillators
{	
	void Oscillator::SetWaveForm(const Waveform& waveform)
	{
		m_waveform = waveform;
	}
	
	void Oscillator::SetSampleRate(const double sampleRate)
	{
		m_samplerate = sampleRate;
		m_deltaTime = 1.0f / sampleRate;
	}

	double Oscillator::Sample(const double frequency, const double level)
	{
		if (m_time >= std::numeric_limits<double>::max())
		{
			m_time = 0;
		}

		double output = 0;
		switch (m_waveform)
		{
		case Waveform::Sin:
		{
			output = level * sin(2 * M_PI * frequency * m_time + m_phase);
			break;
		}
		case Waveform::Square:
		{
			output = level * ((sin(2 * M_PI * frequency * m_time + m_phase)) >= 0.0 ? 1.0 : -1);
			break;
		}
		case Waveform::Triangle:
		{
			double periodTime = 1.0 / frequency;
			double localTime = fmod(m_time, periodTime);

			double value = localTime / periodTime;
			if (value < 0.25)
			{
				output = value * 4;
			}
			else if (value < 0.75)
			{
				output = 2.0 - (value * 4.0);
			}
			else
			{
				output = value * 4 - 4.0;
			}
			break;
		}
		case Waveform::Sawtooth:
		{
			double periodTime = 1.0 / frequency;
			double localTime = fmod(m_time, periodTime);
			output = ((localTime / periodTime) * 2 - 1.0);
			break;
		}
		}

		m_time += m_deltaTime;
		return output;
	}

	void Oscillator::Reset()
	{
		m_time = 0;
	}
}

