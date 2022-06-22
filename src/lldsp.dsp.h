#pragma once
#include "lldsp.utils.h"

namespace lldsp::dsp
{
	class CombFilter
	{
	public:
		CombFilter() : m_SampleDelay(0), m_Gain(1), m_DelayBuffer(44100) {};
		CombFilter(double delayTime, double gain, double sampleRate);

		double Process(double sample);
	private:
		lldsp::utils::RingBuffer m_DelayBuffer;

		int m_SampleDelay;
		double m_Gain;
	};

	class AllPassFilter
	{
	public:
		AllPassFilter() : m_SampleDelay(0), m_Gain(1), m_DelayBuffer(44100) {};
		AllPassFilter(double delayTime, double gain, double sampleRate);

		double Process(double sample);

	private:
		lldsp::utils::RingBuffer m_DelayBuffer;

		int m_SampleDelay;
		double m_Gain;
	};
}