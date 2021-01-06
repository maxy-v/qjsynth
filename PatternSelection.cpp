#include "PatternSelection.h"

PatternSelection::PatternSelection()
{
	invalidate();
}

PatternSelection::PatternSelection(Frame a_first, Frame a_last)
	: m_first(a_first)
	, m_last (a_last )
{
	if (m_first < WrongFrame)
		m_first = WrongFrame;
	if (m_last  < WrongFrame)
		m_last  = WrongFrame;
	
	if (m_last < m_first)
		std::swap(m_first, m_last);
}

PatternSelection PatternSelection::extended(Frame a_frame)
{
	Frame first = a_frame < m_first ? a_frame : m_first;
	Frame last  = a_frame > m_last  ? a_frame : m_last;
	return PatternSelection(first, last);
}

Frame PatternSelection::first() const
{
	return m_first;
}

Frame PatternSelection::last() const
{
	return m_last;
}

Frame PatternSelection::size() const
{
	if (isValid())
	{
		Q_ASSERT(m_last >= m_first);
		return m_last - m_first + 1;
	}
	return 0;
}

bool PatternSelection::isValid() const
{
	return
		m_first >= MinFrame &&
		m_last  >= MinFrame;
}

void PatternSelection::invalidate()
{
	m_first = WrongFrame;
	m_last  = WrongFrame;
}