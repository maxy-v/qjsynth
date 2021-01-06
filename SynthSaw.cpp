#include "SynthSaw.h"

SynthType SynthSaw::type()
{
	return SynthType::Saw;
}

void SynthSaw::generate(double a_phase, double a_frequency)
{
	const double samples_per_phase = Properties::Sound::SampleRate/a_frequency;
	const double phase_offset = a_phase*samples_per_phase;
	const double phase1 = samples_per_phase*0.5; // [ 0   to 0.5 ] -> [  0 to 1 ]
	const double phase2 = samples_per_phase*1.0; // [ 0.5 to 1   ] -> [ -1 to 0 ]

	for (int i = 0; i < Properties::Sound::SamplesPerFrame; i++)
	{
		double phase_sample = fmod(i+phase_offset, samples_per_phase);
		
		if (phase_sample < phase1) 
			setValue(i,        (phase_sample       )/(phase1       ) );
		else
			setValue(i, -1.0 + (phase_sample-phase1)/(phase2-phase1) );
	}
}