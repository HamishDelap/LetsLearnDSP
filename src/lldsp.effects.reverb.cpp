#include "lldsp.effects.reverb.h"
#include "lldsp.dsp.h"

lldsp::effects::Reverb::Reverb(double sampleRate)
{
    m_combOne = lldsp::dsp::CombFilter(m_parameters.t1, m_parameters.g1, sampleRate);
    m_combTwo = lldsp::dsp::CombFilter(m_parameters.t2, m_parameters.g2, sampleRate);
    m_combThree = lldsp::dsp::CombFilter(m_parameters.t3, m_parameters.g3, sampleRate);
    m_combFour = lldsp::dsp::CombFilter(m_parameters.t4, m_parameters.g4, sampleRate);

    m_allPassOne = lldsp::dsp::AllPassFilter(m_parameters.t5, m_parameters.g5, sampleRate);
    m_allPassTwo = lldsp::dsp::AllPassFilter(m_parameters.t6, m_parameters.g6, sampleRate);
}

double lldsp::effects::Reverb::Process(double sample, double time, double amount)
{
    UpdateParameters(time, amount);
    double combFiltersSummed = m_combOne.Process(sample) + m_combTwo.Process(sample) + m_combThree.Process(sample) + m_combFour.Process(sample);
    double firstAllPassResult = m_allPassOne.Process(combFiltersSummed);
    double secondAllPassResult = m_allPassTwo.Process(firstAllPassResult);

    return sample + secondAllPassResult * m_parameters.g7;
}

void lldsp::effects::Reverb::UpdateParameters(double time, double amount)
{
    m_parameters.g7 = amount;

    m_parameters.g1 = std::pow(10, (-1 * (3 * m_parameters.t1) / time));
    m_parameters.g2 = std::pow(10, (-1 * (3 * m_parameters.t2) / time));
    m_parameters.g3 = std::pow(10, (-1 * (3 * m_parameters.t3) / time));
    m_parameters.g4 = std::pow(10, (-1 * (3 * m_parameters.t4) / time));
}
