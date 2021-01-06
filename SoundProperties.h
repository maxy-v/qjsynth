#pragma once
#include "CoreProperties.h"

namespace Properties::Sound
{
	constexpr int SampleRate = 44100;
	constexpr int SamplesPerFrame = SampleRate/Properties::Core::FramesPerSecond;
	constexpr int BytesPerSample = 2;
	constexpr float MainVolume = 0.5f;

	// Checks
	static_assert(SampleRate%Properties::Core::FramesPerSecond == 0);
	static_assert(BytesPerSample == 2); // Expected to be 16 bits per sample
}

static_assert( sizeof(Sample) == Properties::Sound::BytesPerSample );