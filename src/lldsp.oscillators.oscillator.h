#pragma once
#include "lldsp.h"
#include "lldsp.oscillators.h"

namespace lldsp::oscillators
{
	class Oscillator
	{
	public:
		Oscillator() = default;

		void SetWaveForm(const Waveform& waveform);
		void SetSampleRate(const double sampleRate);
		double Sample(const double frequency, const double level);
		void Reset();

	private:
		double m_phase = 0;
		double m_samplerate;
		double m_time;
		double m_deltaTime;
		Waveform m_waveform = Waveform::Sawtooth;
	};
		
}
