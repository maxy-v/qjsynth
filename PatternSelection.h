#pragma once
#include <algorithm>
#include <qglobal.h>
#include "core_types.h"

class PatternSelection
{
public:
	PatternSelection();
	PatternSelection(Frame a_first, Frame a_last);

	PatternSelection extended(Frame a_frame);

	Frame first() const;
	Frame last() const;
	Frame size() const;
	bool  isValid() const;
	void invalidate();

private:
	static constexpr Frame WrongFrame = -1;
	static constexpr Frame MinFrame = 0;
	Frame m_first;
	Frame m_last;
};