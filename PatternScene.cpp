#include "PatternScene.h"

PatternScene::PatternScene()
{
	reset();
}

QGraphicsScene* PatternScene::get()
{
	return &m_scene;
}

QList<PatternItem*> PatternScene::items() const
{
	return itemList( m_scene.items() );
}

QList<PatternItem*> PatternScene::items(int a_first_frame, int a_last_frame) const
{
	Q_ASSERT(a_first_frame >= 0 && a_first_frame < size());
	Q_ASSERT(a_last_frame  >= 0 && a_last_frame  < size());
	Q_ASSERT(a_first_frame <= a_last_frame);
	const int frames = a_last_frame-a_first_frame+1;

	auto items = m_scene.items(
		/* x */ qreal(a_first_frame*Properties::Core::FrameWidth),
		/* y */ qreal(0),
		/* w */ qreal(frames       *Properties::Core::FrameWidth),
		/* h */ m_scene.height(),
		Qt::IntersectsItemBoundingRect,
		Qt::AscendingOrder
	);
	return itemList(items);
}

void PatternScene::deleteItem(PatternItem* a_item)
{
	emit itemDeleted(a_item);
	Q_ASSERT( a_item->scene() == &m_scene );
	QRectF rect = a_item->sceneBoundingRect();
	m_scene.removeItem(a_item);
	delete a_item;

	// Issue: sometimes graphics view keeping "ghost" image of removed item
	// Fix  : force it to repaint item area
	m_scene.invalidate(rect, QGraphicsScene::AllLayers);
}

void PatternScene::addItem(PatternItem* a_item)
{
	Q_ASSERT( a_item->scene() != &m_scene );
	m_scene.addItem(a_item);
	emit itemAdded(a_item);
}

void PatternScene::reset()
{
	auto items = itemList( m_scene.items() );
	for (auto item: items)
		deleteItem(item);
	m_size = Properties::Pattern::DefaultSize;
	updateSceneRect();
}

Frame PatternScene::size() const
{
	return m_size;
}

void PatternScene::setSize(Frame a_size)
{
	m_size = a_size;
	Q_ASSERT(m_size >= Properties::Pattern::MinSize);
	updateSceneRect();
}

QList<PatternItem*> PatternScene::selectedItems() const
{
	auto items = m_scene.selectedItems();
	return itemList(items);
}

void PatternScene::updateSceneRect()
{
	m_scene.setSceneRect(
		0,
		0,
		m_size * Properties::Core::FrameWidth,
		Properties::PianoRoll::Height);
	m_scene.invalidate(QRectF(), QGraphicsScene::BackgroundLayer);
}

QList<PatternItem*> PatternScene::itemList(const QList<QGraphicsItem*>& a_items) const
{
	QList<PatternItem*> items;
	for (auto graphics_item: a_items)
	{
		PatternItem* item = dynamic_cast<PatternItem*>(graphics_item);
		Q_ASSERT(item != nullptr);
		items << item;
	}
	return items;
}
