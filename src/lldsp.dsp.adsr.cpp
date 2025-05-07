#include "lldsp.dsp.adsr.h"

void lldsp::dsp::ADSR::SetSampleRate(const double sampleRate)
{
	m_sampleRate = sampleRate;
}

void lldsp::dsp::ADSR::SetParameters(const Parameters& parameters)
{
	m_parameters = parameters;
	CalculateRates();
}

bool lldsp::dsp::ADSR::IsActive()
{
	return m_state != State::Idle;
}

void lldsp::dsp::ADSR::NoteOn()
{
	if (m_attackRate > 0.0)
	{
		m_state = State::Attack;
	}
	else if (m_decayRate > 0.0)
	{
		m_envelopeValue = 1.0;
		m_state = State::Decay;
	}
	else
	{
		m_envelopeValue = m_parameters.sustain;
		m_state = State::Sustain;
	}
}

void lldsp::dsp::ADSR::NoteOff()
{
	if (m_state != State::Idle)
	{
		if (m_parameters.release > 0.0)
		{
			m_releaseRate = (double)(m_envelopeValue / (m_parameters.release * m_sampleRate));
			m_state = State::Release;
		}
		else
		{
			Reset();
		}
	}
}

double lldsp::dsp::ADSR::Process(double sample)
{
	switch (m_state)
	{
	case State::Idle:
	{
		m_envelopeValue = 0.0;
		break;
	}
	case State::Attack:
	{
		m_envelopeValue += m_attackRate;
		if (m_envelopeValue >= 1.0)
		{
			m_envelopeValue = 1.0;
			NextState();
		}
		break;
	}
	case State::Decay:
	{
		m_envelopeValue = std::max(m_envelopeValue - m_decayRate, m_parameters.sustain);
		if (m_envelopeValue <= m_parameters.sustain)
		{
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
		m_envelopeValue = std::max(0.0, m_envelopeValue - m_releaseRate);
		if (m_envelopeValue <= 0.0)
		{
			NextState();
		}
		break;
	}
	}
	return sample * m_envelopeValue;
}


// If decay == 0 then attackRate should be aiming for sustain level
// If attack == 0 then start at 0?
void lldsp::dsp::ADSR::CalculateRates()
{
	m_attackRate = lldsp::utils::Interpolate(m_parameters.decay == 0 ? m_parameters.sustain : 1.0, m_parameters.attack, m_sampleRate);
	m_decayRate = lldsp::utils::Interpolate(1.0 - m_parameters.sustain, m_parameters.decay, m_sampleRate);
	m_releaseRate = lldsp::utils::Interpolate(m_parameters.sustain, m_parameters.release, m_sampleRate);

	if (m_parameters.attack == 0.0) m_attackRate = 0.0;
	if (m_parameters.decay == 0.0) m_decayRate = 0.0;
	if (m_parameters.release == 0.0) m_releaseRate = 0.0;

	if ((m_state == State::Attack && m_attackRate <= 0.0) ||
		(m_state == State::Decay && (m_decayRate <= 0.0 || m_envelopeValue <= m_parameters.sustain)) ||
		(m_state == State::Release && m_releaseRate <= 0.0))
	{
		NextState();
	}
}

void lldsp::dsp::ADSR::NextState()
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

void lldsp::dsp::ADSR::Reset()
{
	m_envelopeValue = 0;
	m_state = State::Idle;
}
