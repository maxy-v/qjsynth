#include "PatternViewBackgroundPainter.h"
#include "PatternView.h"

PatternViewBackgroundPainter::PatternViewBackgroundPainter(PatternView& a_view)
	: m_view(a_view)
{
}

void PatternViewBackgroundPainter::drawBackground(QPainter* a_painter, const QRectF& a_rect)
{
	a_painter->save();
	QRectF rect = QtTools::limitBySceneRect(&m_view, a_rect);

	// Horizontal lines/rows

	const Key key_index_first = Properties::Piano::KeyCount - ceil (rect.bottom() / Properties::PianoRoll::KeyHeight );
	const Key key_index_last  = Properties::Piano::KeyCount - floor(rect.top()    / Properties::PianoRoll::KeyHeight );
	Q_ASSERT(key_index_first <= key_index_last);
	for (Key key_index = key_index_first; key_index <= key_index_last; key_index++)
	{
		QRectF row_rect(
			rect.x(),
			Properties::PianoRoll::KeyHeight*(Properties::Piano::KeyCount - key_index - 1),
			rect.width(),
			Properties::PianoRoll::KeyHeight
		);
		Key key = key_index%Properties::Piano::KeysInOctave;

		// Key areas

		QColor key_color =
			Properties::Piano::Sharps[key]
			? PatternStyle::SharpKeyColor
			: PatternStyle::KeyColor;
		a_painter->setPen(Qt::NoPen);
		a_painter->setBrush(key_color);
		a_painter->drawRect(row_rect);

		// Key lines

		const QPoint right_fix{1,0}; // It draws 1 extra pixel for some reson
		if (row_rect.top() > 0)
		{
			QRgb color;
			if ((key+1)%Properties::Piano::KeysInOctave == 0)
				color = PatternStyle::DarkLine;
			else
				color = PatternStyle::LightLine;
			a_painter->setPen(color);
			a_painter->drawLine(row_rect.topLeft(), row_rect.topRight()-right_fix);
		}
	}

	// Vertical lines (time)

	static_assert(Properties::Core::SecondWidth % PatternStyle::SecondDivider == 0);
	const Pixel part_width = Properties::Core::SecondWidth/PatternStyle::SecondDivider;

	const Second part_first = rect.left() / part_width;
	const Second part_last  = rect.right()/ part_width;
	Q_ASSERT(part_first <= part_last);
	for (Second part = part_first; part <= part_last; part++)
	{
		QRectF column_rect(
			part_width*part,
			rect.top(),
			part_width,
			rect.height()
		);

		if (column_rect.left() > 0)
		{
			if (part%PatternStyle::SecondDivider == 0)
				a_painter->setPen(PatternStyle::DarkLine);
			else
			{
				if (m_view.transform().m11() <= PatternStyle::IntermediateLineScaleMin)
					// Don't draw intermediate lines on very small scales
					continue;
				else
					a_painter->setPen(PatternStyle::LightLine);
			}
			a_painter->drawLine(column_rect.topLeft(), column_rect.bottomLeft());
		}
	}

	a_painter->restore();
}