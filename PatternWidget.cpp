#include "PatternWidget.h"
#include "ui_PatternWidget.h"

PatternWidget::PatternWidget(QWidget* a_parent)
	: QWidget(a_parent)
	, m_ui(new Ui::PatternWidget)
{
	m_ui->setupUi(this);
	m_ui->grpPianoRoll->setResizeAreaSize(Properties::PianoRoll::ResizeAreaSize);
	m_ui->grpPianoRoll->setTransform( QTransform::fromScale(m_scales.value(), 1) );
	m_ui->grpPianoRoll->setResizeModes(sgv::Right);

	connect(m_ui->grpPianoRoll, &sgv::SignalerGraphicsView::zoomIn    , this, &PatternWidget::onZoomIn);
	connect(m_ui->grpPianoRoll, &sgv::SignalerGraphicsView::zoomOut   , this, &PatternWidget::onZoomOut);
	connect(m_ui->grpPianoRoll, &sgv::SignalerGraphicsView::createItem, this, &PatternWidget::onCreatePatternItem);
	connect(m_ui->grpPianoRoll, &sgv::SignalerGraphicsView::copyItem  , this, &PatternWidget::onCopyPatternItem);
	connect(m_ui->grpPianoRoll, &sgv::SignalerGraphicsView::deleteItem, this, &PatternWidget::onDeletePatternItem);
	connect(m_ui->grpPianoRoll, &sgv::SignalerGraphicsView::resizeItem, this, &PatternWidget::onResizePatternItem);

	connect(m_ui->grpPianoRoll, &PatternView::selectionChanged, this, &PatternWidget::selectionChanged);
	connect(m_ui->grpPianoRoll, &PatternView::pasteRequested  , this, &PatternWidget::onPastePatternItems);
}

void PatternWidget::init(const LibraryModelPtr& a_model, const PatternScenePtr& a_scene, const ClipboardPtr& a_clipboard)
{
	Q_ASSERT( m_library_model.isNull() && !a_model    .isNull() );
	Q_ASSERT( m_scene        .isNull() && !a_scene    .isNull() );
	Q_ASSERT( m_clipboard    .isNull() && !a_clipboard.isNull() );
	m_library_model = a_model;
	m_scene         = a_scene;
	m_clipboard     = a_clipboard;
	
	m_ui->grpPianoRoll->setScene( m_scene->get() );
	m_ui->grpPianoRoll->resetSelection();
}

void PatternWidget::reset()
{
	if (!m_scene.isNull())
		 m_scene->reset();
	resetSelection();
}

void PatternWidget::resetSelection()
{
	m_ui->grpPianoRoll->resetSelection();
}

void PatternWidget::onSizeChanged(int a_size)
{
	m_scene->setSize(a_size);
}

void PatternWidget::onInstrumentSelected(int a_index)
{
	m_index = a_index;
}

void PatternWidget::onInstrumentDeleted(const QModelIndex& a_parent, int a_first, int a_last)
{
	Q_ASSERT(!a_parent.isValid());
	for (int index = a_first; index <= a_last; index++)
	{
		auto deleted_library_item = m_library_model->item(index);

		auto items = m_scene->items();
		for (auto item: items)
		{
			if (item->isSameType(deleted_library_item))
				m_scene->deleteItem(item);
		}
	}
}

void PatternWidget::onInstrumentChanged(const QModelIndex& a_top_left, const QModelIndex& a_bottom_right, const QVector<int>& a_roles)
{
	Q_UNUSED(a_roles);

	if (a_top_left.column()     > LibraryModel::Column::Color ||
		a_bottom_right.column() < LibraryModel::Column::Color )
		return;

	for (int index = a_top_left.row(); index <= a_bottom_right.row(); index++)
	{
		auto changed_library_item = m_library_model->item(index);
		auto items = m_scene->items();
		for (auto item: items)
		{
			if (item->isSameType(changed_library_item))
				item->update();
		}
	}
}

void PatternWidget::onZoomIn()
{
	if (m_scales.prev())
		m_ui->grpPianoRoll->setTransform( QTransform::fromScale(m_scales.value(), 1) );
}

void PatternWidget::onZoomOut()
{
	if (m_scales.next())
		m_ui->grpPianoRoll->setTransform( QTransform::fromScale(m_scales.value(), 1) );
}

void PatternWidget::onCreatePatternItem(const QPointF& a_position)
{
	if (m_index == WrongInstrumentIndex)
		return;

	PatternItem* item = new PatternItem(a_position.toPoint(), m_library_model->item(m_index));
	m_scene->addItem(item);
}

void PatternWidget::onCopyPatternItem(const QPointF& a_position)
{
	auto items = m_scene->selectedItems();
	if (items.size() != 1)
	{
		onCreatePatternItem(a_position);
		return;
	}

	PatternItem* source = items[0];
	PatternItem* item = new PatternItem(a_position.toPoint(), source->libraryItem());
	item->setSize(source->size());
	m_scene->addItem(item);
}

void PatternWidget::onPastePatternItems(const QPointF& a_position)
{
	if (!m_clipboard->isValid())
		return;

	const Frame start_frame = a_position.x() / Properties::Core::FrameWidth;
	for (int i = 0; i < m_clipboard->count(); i++)
	{
		const ClipboardItem& source = m_clipboard->item(i);
		PatternItem* item = new PatternItem(
			source.key(),
			source.frame() + start_frame,
			source.size(),
			source.libraryItem()
		);
		m_scene->addItem(item);
	}
}

void PatternWidget::onDeletePatternItem(QGraphicsItem* a_item)
{
	PatternItem* item = dynamic_cast<PatternItem*>(a_item);
	if (item == nullptr)
		return;

	m_scene->deleteItem(item);
}

void PatternWidget::onResizePatternItem(QGraphicsItem* a_item, sgv::ResizeMode a_mode, const QRectF& a_initial_size, QPointF a_size_delta)
{
	Q_UNUSED(a_mode);

	PatternItem* item = dynamic_cast<PatternItem*>(a_item);
	if (item == nullptr)
		return;

	item->setWidth( static_cast<int>(a_initial_size.width()+a_size_delta.x()) );
}
