#include "SynthTriangle.h"

SynthType SynthTriangle::type()
{
	return SynthType::Triangle;
}

void SynthTriangle::generate(double a_phase, double a_frequency)
{
	const double samples_per_phase = Properties::Sound::SampleRate/a_frequency;
	const double phase_offset = a_phase*samples_per_phase;
	const double phase1 = samples_per_phase*0.25; // [ 0    to 0.25 ] -> [  0 to  1 ]
	const double phase2 = samples_per_phase*0.75; // [ 0.25 to 0.75 ] -> [  1 to -1 ]
	const double phase3 = samples_per_phase*1.00; // [ 0.75 to 1    ] -> [ -1 to  1 ]

	for (int i = 0; i < Properties::Sound::SamplesPerFrame; i++)
	{
		double phase_sample = fmod(i+phase_offset, samples_per_phase);
		
		if (phase_sample < phase1) 
			setValue(i,        (phase_sample       )/(phase1       )     );
		else if (phase_sample < phase2)
			setValue(i,  1.0 - (phase_sample-phase1)/(phase2-phase1)*2.0 );
		else
			setValue(i, -1.0 + (phase_sample-phase2)/(phase3-phase2)     );
	}
}