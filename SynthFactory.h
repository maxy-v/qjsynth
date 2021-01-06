#pragma once
#include "Synth.h"
#include "SynthMode.h"

class SynthFactory
{
public:
	static SynthPtr create(const SynthMode& a_mode);
};