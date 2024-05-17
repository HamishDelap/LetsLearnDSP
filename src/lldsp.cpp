#include "lldsp.h"

namespace lldsp
{
	void ADSR::SetSampleRate(const double sampleRate)
	{
		m_sampleRate = sampleRate;
	}

	void ADSR::SetParameters(const Parameters& parameters)
	{
		m_parameters = parameters;
		CalculateRates();
	}

	void ADSR::NoteOn()
	{
		if (m_attackRate > 0.0)
		{
			m_state = State::Attack;
		}
		else if (m_decayRate > 0.0f)
		{
			m_envelopeValue = 1.0f;
			m_state = State::Decay;
		}
		else
		{
			m_envelopeValue = m_parameters.sustain;
			m_state = State::Sustain;
		}
	}

	void ADSR::NoteOff()
	{
		if (m_state != State::Idle)
		{
			if (m_parameters.release > 0.0f)
			{
				m_releaseRate = (float)(m_envelopeValue / (m_parameters.release * m_sampleRate));
				m_state = State::Release;
			}
			else
			{
				Reset();
			}
		}
	}

	double ADSR::Process(double sample)
	{
		switch (m_state)
		{
		case State::Idle:
			return 0.0;
		case State::Attack:
		{
			m_envelopeValue += m_attackRate;
			if (m_envelopeValue >= 1.0f)
			{
				m_envelopeValue = 1.0f;
				NextState();
			}
			break;
		}
		case State::Decay:
		{
			m_envelopeValue -= m_decayRate;
			if (m_envelopeValue <= m_parameters.sustain)
			{
				m_envelopeValue = m_parameters.sustain;
				NextState();
			}
			break;
		}
		case State::Sustain:
		{
			m_envelopeValue = m_parameters.sustain;
			break;
		}
		case State::Release:
		{
			m_envelopeValue -= m_releaseRate;
			if (m_envelopeValue <= 0.0f)
			{
				NextState();
			}
			break;
		}
		}
		return sample * m_envelopeValue;
	}

	void ADSR::CalculateRates()
	{
		m_attackRate = lldsp::utils::Interpolate(1.0f, m_parameters.attack, m_sampleRate);
		m_decayRate = lldsp::utils::Interpolate(1.0f - m_parameters.sustain, m_parameters.decay, m_sampleRate);
		m_releaseRate = lldsp::utils::Interpolate(m_parameters.sustain, m_parameters.release, m_sampleRate);

		if ((m_state == State::Attack && m_attackRate <= 0.0f) ||
			(m_state == State::Decay && (m_decayRate <= 0.0f || m_envelopeValue <= m_parameters.sustain)) ||
			(m_state == State::Release && m_releaseRate <= 0.0f))
		{
			NextState();
		}
	}

	void ADSR::NextState()
	{
		if (m_state == State::Attack)
		{
			m_state = (m_decayRate > 0.0f ? State::Decay : State::Sustain);
			return;
		}

		if (m_state == State::Decay)
		{
			m_state = State::Sustain;
			return;
		}

		if (m_state == State::Release)
		{
			Reset();
		}
	}

	void ADSR::Reset()
	{
		m_envelopeValue = 0;
		m_state = State::Idle;
	}

}