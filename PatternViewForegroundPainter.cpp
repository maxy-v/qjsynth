#include "PatternViewForegroundPainter.h"
#include "PatternView.h"

PatternViewForegroundPainter::PatternViewForegroundPainter(PatternView& a_view)
	: m_view(a_view)
{
}

void PatternViewForegroundPainter::drawForeground(QPainter* a_painter, const QRectF& a_rect)
{
	a_painter->save();
	QRectF rect = QtTools::limitBySceneRect(&m_view, a_rect);

	if (m_selection.isValid())
	{
		a_painter->setPen(Qt::NoPen);
		a_painter->setBrush( QColor(PatternStyle::SelectionColor) );

		const Frame first_frame = rect.left() /Properties::Core::FrameWidth;
		const Frame last_frame  = rect.right()/Properties::Core::FrameWidth;
		Frame selection_first_frame = m_selection.first();
		Frame selection_last_frame  = m_selection.last();
		if (selection_first_frame <= last_frame  &&
			selection_last_frame  >= first_frame )
		{
			if (selection_first_frame < first_frame)
				selection_first_frame = first_frame;
			if (selection_last_frame  > last_frame)
				selection_last_frame  = last_frame;
			Frame length = selection_last_frame-selection_first_frame+1;
			QRectF selection_rect(
				/* x */ Properties::Core::FrameWidth*selection_first_frame,
				/* y */ rect.top(),
				/* w */ Properties::Core::FrameWidth*length,
				/* h */ rect.bottom()
			);
			if (selection_rect.left()  < rect.left())
				selection_rect.setLeft ( rect.left());
			if (selection_rect.right() > rect.right())
				selection_rect.setRight( rect.right());
			
			a_painter->setOpacity(PatternStyle::SelectionOpacity);
			a_painter->drawRect(selection_rect);
		}
	}

	a_painter->restore();
}

void PatternViewForegroundPainter::onSelectionChanged(const PatternSelection& a_selection)
{
	m_selection = a_selection;
}