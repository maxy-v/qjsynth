#pragma once
#include <qglobal.h>

namespace PatternStyle
{
	// Item

	constexpr quint32 ItemBorderColor         = 0x101010;
	constexpr quint32 SelectedItemBorderColor = 0xD0A0D0;
	constexpr quint32 DefaultItemColor        = 0xAABBDD;

	// Background

	constexpr quint32 KeyColor        = 0x354555;
	constexpr quint32 SharpKeyColor   = 0x30404A;
	constexpr quint32 DarkLine        = 0x102030;
	constexpr quint32 LightLine       = 0x203040;

	constexpr int SecondDivider = 2;
	constexpr float IntermediateLineScaleMin = 0.01f;

	// Selection

	constexpr quint32 SelectionColor = 0x905050;
	constexpr qreal   SelectionOpacity = 0.2;
}