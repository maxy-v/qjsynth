#include "PatternItem.h"

PatternItem::PatternItem(const QPoint& a_point, const LibraryItemCPtr& a_library_item)
	: QGraphicsItem(nullptr)
	, m_key()
	, m_library_item(a_library_item)
{
	commonSetup();
	setPos(snappedPosition(a_point));
}

PatternItem::PatternItem(Key a_key, Frame a_frame, Frame a_size, const LibraryItemCPtr& a_library_item)
	: QGraphicsItem(nullptr)
	, m_key(a_key)
	, m_frame(a_frame)
	, m_size(a_size)
	, m_library_item(a_library_item)
{
	commonSetup();
	setPos(currentPosition());
}

LibraryItemCPtr PatternItem::libraryItem() const
{
	return m_library_item;
}

bool PatternItem::isSameType(const LibraryItemCPtr& a_library_item) const
{
	Q_ASSERT(!a_library_item.isNull());
	Q_ASSERT(!m_library_item.isNull());
	return m_library_item.data() == a_library_item.data();
}

void PatternItem::setWidth(Pixel a_width)
{
	Frame size = a_width/Properties::Core::FrameWidth;
	setSize(size);
}

void PatternItem::setSize(Frame a_size)
{
	if (a_size < 1)
		a_size = 1;
	if (a_size != m_size)
	{
		prepareGeometryChange();
		m_size = a_size;
	}
}

Frame PatternItem::frame() const { return m_frame; }
Key   PatternItem::key()   const { return m_key; }
Frame PatternItem::size()  const { return m_size; }

QRectF PatternItem::boundingRect() const
{
	return QRectF(
		0, 0, 
		m_size * Properties::Core::FrameWidth,
		Properties::PianoRoll::KeyHeight
	);
}

void PatternItem::paint(QPainter* a_painter, const QStyleOptionGraphicsItem* a_option, QWidget* a_widget)
{
	Q_UNUSED(a_widget);
	if (m_library_item.isNull())
		return;

	a_painter->save();

	auto item = m_library_item.get();
	const QRectF rect = boundingRect()
		.adjusted(0,0,-1,-1); // For some readon Qt draws it 1 pixel taller and wider

	// Rect

	a_painter->setBrush( item->color() );
	if (a_option->state.testFlag(QStyle::State_Selected))
		a_painter->setPen(PatternStyle::SelectedItemBorderColor);
	else
		a_painter->setPen(PatternStyle::ItemBorderColor);
	a_painter->drawRect( rect );

	// Text

	double scale_x = scale()/a_painter->transform().m11();
	QString name = Properties::Piano::keyName(m_key);
	if (a_painter->fontMetrics().width(name) < rect.width()/scale_x)
	{
		a_painter->scale(scale_x, 1.0);
		a_painter->setFont( QFont("Courier New", 8) );
		a_painter->setPen(Qt::black);
		a_painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, name);
	}

	a_painter->restore();
}

QVariant PatternItem::itemChange(QGraphicsItem::GraphicsItemChange a_change, const QVariant& a_value)
{
	if (a_change == ItemPositionChange && scene() != nullptr)
	{
		// Issue: sometimes graphics view keeping "ghost" image of moved item
		// Fix  : force it to repaint previous item area
		scene()->invalidate(sceneBoundingRect(), QGraphicsScene::AllLayers);

		return snappedPosition( a_value.toPoint() );
	}

	return QGraphicsItem::itemChange(a_change, a_value);
}

void PatternItem::commonSetup()
{
	Q_ASSERT(m_size >= 1);
	setFlags(
		QGraphicsItem::ItemIsSelectable        |
		QGraphicsItem::ItemIsMovable           |
		QGraphicsItem::ItemSendsGeometryChanges // For handling ItemPositionChange event
	);
}

QPoint PatternItem::currentPosition()
{
	return {
		m_frame                            * Properties::Core::FrameWidth,
		(Properties::Piano::LastKey - m_key) * Properties::PianoRoll::KeyHeight
	};
}

QPoint PatternItem::snappedPosition(const QPoint& a_point)
{
	m_frame  = a_point.x() / Properties::Core::FrameWidth;
	m_key    = Properties::Piano::LastKey - a_point.y()/Properties::PianoRoll::KeyHeight;

	if (m_frame < 0)
		m_frame = 0;
	if (m_key < Properties::Piano::FirstKey)
		m_key = Properties::Piano::FirstKey;
	else
	if (m_key > Properties::Piano::LastKey)
		m_key = Properties::Piano::LastKey;
	
	return currentPosition();
}
