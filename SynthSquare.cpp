#include "SynthSquare.h"

SynthSquare::SynthSquare(float a_duty)
{
	m_duty = a_duty;
}

SynthType SynthSquare::type()
{
	return SynthType::Square;
}

void SynthSquare::generate(double a_phase, double a_frequency)
{
	const double samples_per_phase = Properties::Sound::SampleRate/a_frequency;
	const double phase_offset = a_phase*samples_per_phase;
	const double phase_low = samples_per_phase*m_duty;

	for (int i = 0; i < Properties::Sound::SamplesPerFrame; i++)
	{
		double phase_sample = fmod(i+phase_offset, samples_per_phase);
		if (phase_sample < phase_low)
			setValue(i, -1.0);
		else
			setValue(i, +1.0);
	}
}