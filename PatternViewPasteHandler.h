#pragma once
#include <QMouseEvent>
#include <QPainter>
#include "QtTools.h"
#include "PatternSelection.h"

class PatternViewPasteHandler
{
public:
	PatternViewPasteHandler(class PatternView& a_view);

	void mouseReleaseEvent(QMouseEvent* a_event);

private:
	class PatternView& m_view;
};