#pragma once
#include "lldsp.h"
#include "lldsp.utils.h"
#include "lldsp.dsp.h"
#include "lldsp.effects.granular.h"

// Effects
namespace lldsp::effects
{
	// Distortion
    double TanhWaveshaper(double sample, double gain);
	double DafxDistortion(double sample, double gain);
	double Overdrive(double sample, double gain);
}