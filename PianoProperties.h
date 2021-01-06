#pragma once
#include <cmath>
#include <cstdio>
#include "core_types.h"

namespace Properties::Piano
{
	constexpr Octave FirstOctave  = 2_oct;
	constexpr Octave LastOctave   = 7_oct;
	constexpr Octave OctaveCount  = LastOctave - FirstOctave + 1;
	constexpr Key    KeysInOctave = 12_key;
	constexpr Key    KeyCount     = OctaveCount * KeysInOctave;
	constexpr Key    FirstKey     = FirstOctave * KeysInOctave;
	constexpr Key    LastKey      = FirstKey + KeyCount - 1;
	constexpr bool   Sharps[KeysInOctave] = {0,1,0,1,0,0,1,0,1,0,1,0};
	
	double frequency(double a_key);
	const char* keyName(Key a_key_index);

	// Checks
	static_assert(FirstOctave >= 0_oct);
	static_assert(LastOctave  <= 10_oct);
	static_assert(LastOctave  >= FirstOctave);
};