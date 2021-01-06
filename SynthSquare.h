#pragma once
#include "Synth.h"
#include <cmath>

class SynthSquare: public Synth
{
public:
	SynthSquare() = default;
	SynthSquare(float a_duty);

	virtual SynthType type() override;
	virtual void generate(double a_phase, double a_frequency) override;
private:
	float m_duty = 0.5;
};