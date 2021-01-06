#include "PatternView.h"

PatternView::PatternView(QWidget* a_parent)
	: sgv::SignalerGraphicsView(a_parent)
	, m_background_painter(*this)
	, m_foreground_painter(*this)
	, m_selection_handler (*this)
	, m_paste_handler     (*this)
{
	connect(this, &PatternView::selectionChanged, &m_foreground_painter, &PatternViewForegroundPainter::onSelectionChanged);
}

void PatternView::resetSelection()
{
	m_selection_handler.reset();
}

void PatternView::drawBackground(QPainter* a_painter, const QRectF& a_rect)
{
	SignalerGraphicsView::drawBackground(a_painter, a_rect);
	m_background_painter .drawBackground(a_painter, a_rect);
}

void PatternView::drawForeground(QPainter* a_painter, const QRectF& a_rect)
{
	SignalerGraphicsView::drawForeground(a_painter, a_rect);
	m_foreground_painter .drawForeground(a_painter, a_rect);
}

void PatternView::mousePressEvent(QMouseEvent* a_event)
{
	m_selection_handler  .mousePressEvent(a_event);
	SignalerGraphicsView::mousePressEvent(a_event);
}

void PatternView::mouseMoveEvent(QMouseEvent* a_event)
{
	m_selection_handler  .mouseMoveEvent(a_event);
	SignalerGraphicsView::mouseMoveEvent(a_event);
}

void PatternView::mouseReleaseEvent(QMouseEvent* a_event)
{
	m_selection_handler  .mouseReleaseEvent(a_event);
	m_paste_handler      .mouseReleaseEvent(a_event);
	SignalerGraphicsView::mouseReleaseEvent(a_event);
}
