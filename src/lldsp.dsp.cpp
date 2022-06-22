#include "lldsp.dsp.h"

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