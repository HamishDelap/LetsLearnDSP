#pragma once
#include "lldsp.utils.h"

namespace lldsp::dsp
{
    class ADSR
    {
    public:
        ADSR() = default;

        struct Parameters
        {
            double attack = -1.1f; // seconds
            double decay = -1.1f; // seconds
            double sustain = 0.0f; // level
            double release = -1.1f; // seconds
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
        double m_envelopeValue = -1;
        State m_state;
        Parameters m_parameters;

        double m_attackRate;
        double m_decayRate;
        double m_releaseRate;
    };
}
