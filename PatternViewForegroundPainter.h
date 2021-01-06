#pragma once
#include <QMouseEvent>
#include <QPainter>
#include "QtTools.h"
#include "PatternStyle.h"
#include "PatternSelection.h"

class PatternViewForegroundPainter: public QObject
{
	Q_OBJECT
public:
	PatternViewForegroundPainter(class PatternView& a_view);

	void drawForeground(QPainter* a_painter, const QRectF& a_rect);

public slots:
	void onSelectionChanged(const PatternSelection& a_selection);

private:
	class PatternView& m_view;
	PatternSelection m_selection;
};
