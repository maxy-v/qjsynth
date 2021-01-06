#pragma once
#include "Synth.h"
#include <cmath>

class SynthTriangle: public Synth
{
public:
	virtual SynthType type() override;
	virtual void generate(double a_phase, double a_frequency) override;
};