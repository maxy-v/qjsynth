#include "PatternViewPasteHandler.h"
#include "PatternView.h"

PatternViewPasteHandler::PatternViewPasteHandler(PatternView& a_view)
	: m_view(a_view)
{
}

void PatternViewPasteHandler::mouseReleaseEvent(QMouseEvent* a_event)
{
	if (a_event->button()    == Qt::MiddleButton &&
		a_event->modifiers() == Qt::ControlModifier)
		emit m_view.pasteRequested( QtTools::graphicsScenePosition(a_event, &m_view) );
}