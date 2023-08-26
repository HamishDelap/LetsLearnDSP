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

	// Based on: https://www.earlevel.com/main/2003/02/28/biquads/ 
	class BiQuadFilter
	{
	public:
		BiQuadFilter() : m_sampleRate(44100) {}
		BiQuadFilter(double sampleRate) : m_sampleRate(sampleRate) {}
		
		double Process(double sample);

		void SetCutoff(double Fc, double Q, double peakGain);

	private:
		double m_sampleRate;
		double m_z1 = 0;
		double m_z2 = 0;
		double m_a0 = 0.03641371651958762;
		double m_a1 = 0.07282743303917524;
		double m_a2 = 0.03641371651958762;
		double m_b1 = -1.4440155783021371;
		double m_b2 = 0.5896704443804874;
	};
}