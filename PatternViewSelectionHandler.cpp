#include "PatternViewSelectionHandler.h"
#include "PatternView.h"

PatternViewSelectionHandler::PatternViewSelectionHandler(PatternView& a_view)
	: m_view(a_view)
{
}

void PatternViewSelectionHandler::reset()
{
	const PatternSelection previous = m_selection;
	m_selection.invalidate();
	redrawSelections(previous, m_selection);
	emit m_view.selectionChanged(m_selection);
}

void PatternViewSelectionHandler::mousePressEvent(const QMouseEvent* a_event)
{
	if (isSelectionDefiningEvent(a_event))
	{
		const PatternSelection previous = m_selection;
		Frame x = QtTools::graphicsScenePosition(a_event, &m_view).x();
		if (x > m_view.sceneRect().width())
			x = m_view.sceneRect().width();
		Frame frame = x/Properties::Core::FrameWidth;
		m_selection = PatternSelection(frame, frame);
		redrawSelections(previous, m_selection);
		emit m_view.selectionChanged(m_selection);
	}
	else if (isSelectionClearingEvent(a_event))
	{
		const PatternSelection previous = m_selection;
		m_selection.invalidate();
		redrawSelections(previous, m_selection);
		emit m_view.selectionChanged(m_selection);
	}
}

void PatternViewSelectionHandler::mouseMoveEvent(const QMouseEvent* a_event)
{
	if (isSelectionDefiningEvent(a_event))
	{
		const PatternSelection previous = m_selection;
		Frame x = QtTools::graphicsScenePosition(a_event, &m_view).x();
		if (x > m_view.sceneRect().width())
			x = m_view.sceneRect().width();
		Frame last = x/Properties::Core::FrameWidth;
		if ( last != m_selection.last() && last >= 0)
		{
			m_selection = m_selection.extended(last);
			redrawSelections(previous, m_selection);
			emit m_view.selectionChanged(m_selection);
		}
	}
}

void PatternViewSelectionHandler::mouseReleaseEvent(const QMouseEvent* a_event)
{
	if (isSelectionDefiningEvent(a_event))
	{
		const PatternSelection previous = m_selection;
		Frame x = QtTools::graphicsScenePosition(a_event, &m_view).x();
		if (x > m_view.sceneRect().width())
			x = m_view.sceneRect().width();
		Frame last = x/Properties::Core::FrameWidth;
		m_selection = PatternSelection(m_selection.first(), last);
		if (m_selection.size() == 1)
			m_selection.invalidate();
		redrawSelections(previous, m_selection);
		emit m_view.selectionChanged(m_selection);
	}
}

bool PatternViewSelectionHandler::isSelectionDefiningEvent(const QMouseEvent* a_event)
{
	return
		a_event->buttons().testFlag(Qt::MiddleButton) &&
		a_event->modifiers() == Qt::NoModifier;
}

bool PatternViewSelectionHandler::isSelectionClearingEvent(const QMouseEvent* a_event)
{
	return
		a_event->buttons().testFlag(Qt::MiddleButton) &&
		a_event->modifiers() == Qt::AltModifier;
}

void PatternViewSelectionHandler::redrawSelections(const PatternSelection& a_first, const PatternSelection& a_second)
{
	redrawSelection(a_first);
	redrawSelection(a_second);
}

void PatternViewSelectionHandler::redrawSelection(const PatternSelection& a_selection)
{
	if (!a_selection.isValid())
		return;

	QRectF rect(
		/* x */ Properties::Core::FrameWidth*a_selection.first(),
		/* y */ 0,
		/* w */ Properties::Core::FrameWidth*a_selection.size(),
		/* h */ m_view.sceneRect().bottom()
	);
	if (m_view.scene() != nullptr)
		m_view.scene()->invalidate(rect, QGraphicsScene::ForegroundLayer);
}