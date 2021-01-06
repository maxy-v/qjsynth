#pragma once
#include <cstdint>

using Pixel  = int;
using Frame  = int;
using Octave = int;
using Key    = int;
using Second = int;
using Sample = int16_t;

constexpr Pixel operator""_pix(unsigned long long a_value) {
	return Pixel{ static_cast<int>(a_value) };
}

constexpr Frame operator""_frm(unsigned long long a_value) {
	return Frame{ static_cast<int>(a_value) };
}

constexpr Octave operator""_oct(unsigned long long a_value) {
	return Octave{ static_cast<int>(a_value) };
}

constexpr Key operator""_key(unsigned long long a_value) {
	return Key{ static_cast<int>(a_value) };
}

constexpr Second operator""_sec(unsigned long long a_value) {
	return Second{ static_cast<int>(a_value) };
}