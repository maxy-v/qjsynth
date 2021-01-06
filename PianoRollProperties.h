#pragma once
#include "PianoProperties.h"
#include "CoreProperties.h"
#include "PatternProperties.h"

namespace Properties::PianoRoll
{
	constexpr Pixel KeyHeight      = Properties::Core::FrameWidth;
	constexpr Pixel OctaveHeight   = Properties::Piano::KeysInOctave * KeyHeight;
	constexpr Pixel Height         = Properties::Piano::KeyCount     * KeyHeight;
	constexpr Pixel DefaultWidth   = Properties::Core::FrameWidth * Properties::Pattern::DefaultSize;
	constexpr Pixel ResizeAreaSize = static_cast<Pixel>( 0.3*Properties::Core::FrameWidth );

	// Checks
	static_assert(ResizeAreaSize >= 3, "ResizeArea is too small");
}