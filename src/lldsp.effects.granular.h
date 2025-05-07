#pragma once
#include <vector>
#include <deque>

namespace lldsp::effects
{
	using GrainData = std::vector<double>;
	// Class to represent a single grain.
	class Grain
	{
	public:
		Grain();
		Grain(GrainData& data);
		int PushSample(const double& sample);
		void ApplyHanning();
		GrainData Process();
		double GetSample(int index);
		int GetSize();
		void Clear();
	private:

		size_t m_size = 0;
		GrainData m_vData;
	};

	// Class to store a delayline of grains and control the filling and fetching of grains
	class GranularDelayLine
	{
	public:
		void SetGrainCount(int grainCount);
		void SetGrainSize(int grainSize, int sampleRate); // In ms
		double Process(double sample);
	
	private:
		void AddSample(double& sample);
		int PushGrain();

		std::deque<Grain> m_grains;
		Grain m_grainInProgress;
		int m_grainSize;
		int m_maxGrainCount = 0;

		Grain m_currentGrain;
		int m_currentGrainIndex = 0;
		int m_currentGrainSample = 0;
		
		Grain m_secondGrain;
		int m_secondGrainIndex = 1;
		int m_secondGrainSample = 0;
		
		bool m_isFull = false;

		bool m_crossFadeStart = false;
	};
}
