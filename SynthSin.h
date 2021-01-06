#pragma once
#include "Synth.h"

class SynthSin: public Synth
{
public:
	virtual SynthType type() override;
	virtual void generate(double a_phase, double a_frequency) override;
};