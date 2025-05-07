#pragma once
#include "lldsp.utils.h"
#include "lldsp.dsp.h"

// Effects
namespace lldsp::effects
{
	// Distortion
    double TanhWaveshaper(double sample, double gain);
	double DafxDistortion(double sample, double gain);
	double Overdrive(double sample, double gain);
}