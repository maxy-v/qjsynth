#pragma once
#include "core_types.h"

namespace Properties::Core
{
	constexpr Pixel  FrameWidth      = 12_pix;
	constexpr Frame  FramesPerSecond = 60_frm;
	constexpr Second SecondsInMinute = 60_sec;
	constexpr Pixel  SecondWidth     = FrameWidth*FramesPerSecond;
}