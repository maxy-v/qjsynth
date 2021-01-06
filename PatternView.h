#pragma once
#include "properties.h"
#include "QtTools.h"
#include "SignalerGraphicsView/SignalerGraphicsView.h"
#include "PatternViewBackgroundPainter.h"
#include "PatternViewForegroundPainter.h"
#include "PatternViewSelectionHandler.h"
#include "PatternViewPasteHandler.h"

class PatternView: public sgv::SignalerGraphicsView
{
	Q_OBJECT
public:
	PatternView(QWidget* a_parent = nullptr);

	void resetSelection();

signals:
	void selectionChanged(const PatternSelection& a_selection);
	void pasteRequested(const QPoint& a_position);

protected:
	virtual void drawBackground(QPainter* a_painter, const QRectF& a_rect) override;
	virtual void drawForeground(QPainter* a_painter, const QRectF& a_rect) override;
	virtual void mousePressEvent  (QMouseEvent* a_event) override;
	virtual void mouseMoveEvent   (QMouseEvent* a_event) override;
	virtual void mouseReleaseEvent(QMouseEvent* a_event) override;

private:
	PatternViewBackgroundPainter m_background_painter;
	PatternViewForegroundPainter m_foreground_painter;
	PatternViewSelectionHandler  m_selection_handler;
	PatternViewPasteHandler      m_paste_handler;
};
