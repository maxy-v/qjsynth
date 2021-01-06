#include "QtTools.h"

void QtTools::centerWidgetOnCursor(QWidget* a_widget)
{
	QTimer::singleShot(0, [a_widget]() {
		QPoint position = QCursor::pos() - a_widget->rect().center();
		if (position.x() < 0)
			position.setX(0);
		if (position.y() < 0)
			position.setY(0);
		a_widget->move(position);
	});
}

void QtTools::setTabSize(QTextEdit* a_edit, int a_size)
{
	QFontMetrics metrics(a_edit->font());
	a_edit->setTabStopDistance( a_size * metrics.horizontalAdvance(' ') );
}

void QtTools::setLayoutItemsEnabled(QLayout* a_layout, bool a_enabled)
{
	for (int i = 0; i < a_layout->count(); i++)
	{
		QWidget* widget = a_layout->itemAt(i)->widget();
		if (widget != nullptr)
			widget->setEnabled(a_enabled);
	}
}

void QtTools::setLayoutItemsVisible(QLayout* a_layout, bool a_visible)
{
	for (int i = 0; i < a_layout->count(); i++)
	{
		QWidget* widget = a_layout->itemAt(i)->widget();
		if (widget != nullptr)
			widget->setVisible(a_visible);
	}
}

QPoint QtTools::graphicsScenePosition(const QMouseEvent* a_event, const QGraphicsView* a_view)
{
	QPoint local_pos = a_event->localPos().toPoint();
	QPoint scene_pos = a_view->mapToScene(local_pos).toPoint();
	return scene_pos;
}

QPoint QtTools::graphicsScenePosition(const QWheelEvent* a_event, const QGraphicsView* a_view)
{
	QPoint local_pos = a_event->position().toPoint();
	QPoint scene_pos = a_view->mapToScene(local_pos).toPoint();
	return scene_pos;
}

QRectF QtTools::limitBySceneRect(const QGraphicsView* a_view, const QRectF& a_rect)
{
	QRectF rect = a_rect;
	if (a_view->scene() != nullptr)
	{
		QPointF scene_end  = a_view->scene()->sceneRect().bottomRight();
		if (rect.right()  > scene_end.x())
			rect.setRight ( scene_end.x());
		if (rect.bottom() > scene_end.y())
			rect.setBottom( scene_end.y());
	}
	return rect.normalized();
}
