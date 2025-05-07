#include "lldsp.effects.granular.h"
#include "lldsp.utils.h"
#include "lldsp.math.h"

using namespace lldsp::effects;

Grain::Grain() : m_vData()
{
}

Grain::Grain(GrainData& data) : m_vData(data)
{
	m_size = m_vData.size();
}

int Grain::PushSample(const double& sample)
{
	m_vData.push_back(sample);
	m_size = m_vData.size();
	return static_cast<int>(m_vData.size());
}

int Grain::GetSize()
{
	return static_cast<int>(m_size);
}

void Grain::ApplyHanning()
{
	lldsp::utils::HanningWindow(m_vData);
}

GrainData Grain::Process()
{
	return m_vData;
}

double Grain::GetSample(int index)
{
	if (index >= m_size)
	{
		return 0;
	}
	return m_vData.at(index);
}

void Grain::Clear()
{
	m_vData.clear();
	m_size = 0;
}


void GranularDelayLine::SetGrainCount(int grainCount)
{
	m_maxGrainCount = grainCount;
}

void GranularDelayLine::SetGrainSize(int grainSize, int sampleRate)
{
	m_grainSize = static_cast<int>(std::floor(grainSize * 0.001) * sampleRate);
}

void GranularDelayLine::AddSample(double& sample)
{
	int grainSize = m_grainInProgress.PushSample(sample);
	if (grainSize >= m_grainSize)
	{
		PushGrain();
		m_grainInProgress.Clear();
	}
}

int GranularDelayLine::PushGrain()
{
	m_grainInProgress.ApplyHanning(); // Apply Window
	m_grains.push_back(m_grainInProgress);
	if (m_grains.size() > m_maxGrainCount)
	{
		m_isFull = true;
		m_grains.pop_front();
	}
	return static_cast<int>(m_grains.size());
}

double GranularDelayLine::Process(double sample)
{
	AddSample(sample);

	if (!m_isFull)
	{
		return 0;
	}

	m_currentGrain = m_grains.at(m_currentGrainIndex);
	m_secondGrain = m_grains.at(m_secondGrainIndex);

	if (m_currentGrainSample > m_currentGrain.GetSize() - 1)
	{
		m_currentGrainIndex += rand() % 3;
		m_currentGrainSample = 0;
	}
	if (m_currentGrainIndex >= m_grains.size())
	{
		m_currentGrainIndex = 0;
	}

	if (m_currentGrainIndex == m_currentGrain.GetSize() / 2)
	{
		m_crossFadeStart = true;
	}

	if (m_secondGrainSample > m_secondGrain.GetSize() - 1)
	{
		m_secondGrainIndex += rand() % 3;
		m_secondGrainSample = 0;
	}
	if (m_secondGrainIndex >= m_grains.size())
	{
		m_secondGrainIndex = 1;
	}

	if (!m_crossFadeStart)
	{
		return m_currentGrain.GetSample(m_currentGrainSample++);
	}
	return std::tanh(m_currentGrain.GetSample(m_currentGrainSample++) + m_secondGrain.GetSample(m_secondGrainSample));

}
