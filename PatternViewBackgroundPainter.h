#pragma once
#include <QMouseEvent>
#include <QPainter>
#include "QtTools.h"
#include "PatternStyle.h"

class PatternViewBackgroundPainter
{
public:
	PatternViewBackgroundPainter(class PatternView& a_view);
	
	void drawBackground(QPainter* a_painter, const QRectF& a_rect);

private:
	class PatternView& m_view;
};