#pragma once
#include <QMouseEvent>
#include <QPainter>
#include "QtTools.h"
#include "PatternSelection.h"

class PatternViewSelectionHandler
{
public:
	PatternViewSelectionHandler(class PatternView& a_view);

	void reset();

	void mousePressEvent  (const QMouseEvent* a_event);
	void mouseMoveEvent   (const QMouseEvent* a_event);
	void mouseReleaseEvent(const QMouseEvent* a_event);

private:
	bool isSelectionDefiningEvent(const QMouseEvent* a_event);
	bool isSelectionClearingEvent(const QMouseEvent* a_event);
	void redrawSelections(const PatternSelection& a_first, const PatternSelection& a_second);
	void redrawSelection (const PatternSelection& a_selection);

	PatternSelection m_selection;
	class PatternView& m_view;
};