#include "SynthSin.h"

SynthType SynthSin::type()
{
	return SynthType::Sin;
}

void SynthSin::generate(double a_phase, double a_frequency)
{
	constexpr double pi = 3.14159265358979323846;

	for (int i = 0; i < Properties::Sound::SamplesPerFrame; i++)
	{
		double value = sin(
				(
					360*a_phase + 
						i*(a_frequency/Properties::Core::FramesPerSecond)
						*360/double{Properties::Sound::SamplesPerFrame}
				) * pi/180
			);
		setValue(i, value);
	}
}