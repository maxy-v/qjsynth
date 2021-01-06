#pragma once
#include "CoreProperties.h"
#include "core_types.h"

namespace Properties::Pattern
{
	constexpr Second DefaultDuration = 5_sec;
	constexpr Second MaxDuration     = 300_sec; // 5 minutes
	constexpr Frame  MinSize         = 0_frm;
	constexpr Frame  MaxSize         = Properties::Core::FramesPerSecond * MaxDuration;
	constexpr Frame  DefaultSize     = Properties::Core::FramesPerSecond * DefaultDuration;
	constexpr int    DefaultItemSizeDivider = 10;
	constexpr Frame  DefaultItemSize        = Properties::Core::FramesPerSecond / DefaultItemSizeDivider;

	// Checks
	static_assert( MinSize >= 0_frm );
	static_assert( MinSize <= MaxSize );
	static_assert( DefaultSize >= MinSize );
	static_assert( DefaultSize <= MaxSize );
	static_assert( Properties::Core::FramesPerSecond % DefaultItemSizeDivider == 0 );
};