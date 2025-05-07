#pragma once
#include "lldsp.oscillators.waveforms.h"

namespace lldsp::oscillators
{
	class Oscillator
	{
	public:
		Oscillator() = default;

		void SetWaveForm(const Waveform& waveform);
		void SetSampleRate(const double sampleRate);
		double Sample(const double frequency, const double level);
		void Reset(); // Helper function for debugging

	private:
		double m_phase = 0;
		double m_samplerate;
		Waveform m_waveform = Waveform::Sawtooth;
	};

}
