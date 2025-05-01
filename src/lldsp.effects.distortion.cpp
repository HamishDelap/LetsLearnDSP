#include "lldsp.effects.h"
#include "lldsp.utils.h"

namespace lldsp::effects
{
    double TanhWaveshaper(double sample, double gain)
    {
        return std::tanh(sample * gain) / gain;
    }

    double DafxDistortion(double sample, double gain)
    {
        if (sample == 0.0) { return 0; }
        sample *= gain;
        double signPreservation = sample / std::abs(sample);
        return (signPreservation * (1 - std::exp(((sample * sample)) / std::abs(sample)))) * gain;
    }

    double Overdrive(double sample, double gain)
    {
        sample *= gain;
        double absolute = std::abs(sample);

        if (absolute < 0.333333333333)
        {
            return sample * 2;
        }
        else if (absolute < 0.6666666666)
        {
            return (3 - ((2 - 3 * sample) * (2 - 3 * sample))) / 3;
        }
        return sample;
    }
}
