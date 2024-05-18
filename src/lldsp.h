#define _USE_MATH_DEFINES

#ifndef _LLDSP_LIB_
#define _LLDSP_LIB_

#include <cmath>
#include <math.h>
#include "lldsp.utils.h"

namespace lldsp
{
	class ADSR
	{
	public:
		ADSR() = default;

		struct Parameters
		{
			double attack = 0.1f; // seconds
			double decay = 0.1f; // seconds
			double sustain = 1.0f; // level
			double release = 0.1f; // seconds
		};

		enum class State
		{
			Idle,
			Attack,
			Decay,
			Sustain,
			Release
		};

		void SetSampleRate(const double sampleRate);
		void SetParameters(const Parameters& parameters);
	
		bool IsActive();

		void NoteOn();
		void NoteOff();

		double Process(double sample);

	private:
		void CalculateRates();
		void NextState();
		void Reset();

		double m_sampleRate;
		double m_envelopeValue = 0;
		State m_state;
		Parameters m_parameters;

		double m_attackRate;
		double m_decayRate;
		double m_releaseRate;
	};
}

#endif