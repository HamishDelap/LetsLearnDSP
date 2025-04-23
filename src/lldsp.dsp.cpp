#include "lldsp.dsp.h"

#include <math.h>
#include <cmath>

// CombFilter
lldsp::dsp::CombFilter::CombFilter(double delayTime, double gain, double sampleRate) : m_DelayBuffer(sampleRate)
{
	m_Gain = gain;
	m_SampleDelay = sampleRate * delayTime;
}

double lldsp::dsp::CombFilter::Process(double sample)
{
	double delayedVal = m_DelayBuffer.Get(m_SampleDelay);

	double valToPush = sample + (delayedVal * m_Gain);
	m_DelayBuffer.Push(valToPush);

	return delayedVal;
}

//AllPassFilter
lldsp::dsp::AllPassFilter::AllPassFilter(double delayTime, double gain, double sampleRate) : m_DelayBuffer(sampleRate)
{
	m_Gain = gain;
	m_SampleDelay = sampleRate * delayTime;
}

double lldsp::dsp::AllPassFilter::Process(double sample)
{
	double delayedVal = m_DelayBuffer.Get(m_SampleDelay);

	double valToPush = sample + (delayedVal * m_Gain);
	m_DelayBuffer.Push(valToPush);

	double postDelayVal = delayedVal * (1 - m_Gain * m_Gain);

	return postDelayVal + sample * (-1 * m_Gain);
}


void lldsp::dsp::BiQuadFilter::SetSampleRate(double sampleRate)
{
	m_sampleRate = sampleRate;
}

double lldsp::dsp::BiQuadFilter::Process(double sample)
{
	double output = (sample * m_a0) + m_z1;
	m_z1 = (sample * m_a1) + m_z2 - (output * m_b1);
	m_z2 = (sample * m_a2) - (output * m_b2);
	return output;
}

void lldsp::dsp::BiQuadFilter::SetCutoff(double Fc, double Q, double peakGain)
{
	if (Fc < 10) { Fc = 10; }
	double norm;
	//double V = std::pow(10, peakGain / 20);
	double K = tan(M_PI * Fc / m_sampleRate);
	norm = 1 / (1 + K / Q + K * K);
	m_a0 = K * K * norm;
	m_a1 = 2 * m_a0;
	m_a2 = m_a0;
	m_b1 = 2 * (K * K - 1) * norm;
	m_b2 = (1 - K / Q + K * K) * norm;
}
