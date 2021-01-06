#pragma once
#include <QWidget>
#include <QTimer>
#include <QCursor>
#include <QTextEdit>
#include <QLayOut>
#include <QMouseEvent>
#include <QGraphicsView>

class QtTools
{
public:
	static void centerWidgetOnCursor(QWidget* a_widget);
	static void setTabSize(QTextEdit* a_edit, int a_size);
	static void setLayoutItemsEnabled(QLayout* a_layout, bool a_enabled);
	static void setLayoutItemsVisible(QLayout* a_layout, bool a_visible);
	static QPoint graphicsScenePosition(const QMouseEvent* a_event, const QGraphicsView* a_view);
	static QPoint graphicsScenePosition(const QWheelEvent* a_event, const QGraphicsView* a_view);
	static QRectF limitBySceneRect(const QGraphicsView* a_view, const QRectF& a_rect);
};