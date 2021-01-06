#include "LibraryWidget.h"
#include "ui_LibraryWidget.h"

LibraryWidget::LibraryWidget(QWidget* a_parent) :
	QWidget(a_parent),
	m_ui(new Ui::LibraryWidget)
{
	m_ui->setupUi(this);

	connect(m_ui->btnAdd   , &QPushButton::clicked, this, &LibraryWidget::onAdd);
	connect(m_ui->btnDelete, &QPushButton::clicked, this, &LibraryWidget::onDelete);

	m_ui->lstItems->setItemDelegate(&m_delegate);
	m_ui->lstItems->installEventFilter(this);
	connect(m_ui->lstItems, &QListView::customContextMenuRequested, this, &LibraryWidget::onContextMenuRequested);
}

void LibraryWidget::init(const LibraryModelPtr& a_model)
{
	Q_ASSERT( m_model.isNull());
	Q_ASSERT(!a_model.isNull());
	m_model = a_model;

	m_ui->lstItems->setModel( m_model.get() );
	connect(m_ui->lstItems->selectionModel(), &QItemSelectionModel::selectionChanged, this, &LibraryWidget::onSelectionChanged);

	connect(m_model.get(), &QAbstractItemModel::rowsInserted, this, &LibraryWidget::updateDeleteButtonState);
	connect(m_model.get(), &QAbstractItemModel::rowsRemoved , this, &LibraryWidget::updateDeleteButtonState);
}

bool LibraryWidget::eventFilter(QObject* a_watched, QEvent* a_event)
{
	if (a_watched == m_ui->lstItems)
	{
		if (a_event->type() == QEvent::KeyPress)
		{
			const QKeyEvent* event = static_cast<QKeyEvent*>(a_event);
			const int  key = event->key();
			const auto mod = event->modifiers();
			
			if      (key == Qt::Key_Delete && mod == Qt::NoModifier)
			{
				deleteSelectedItem();
				return true;
			}
			else if (key == Qt::Key_Insert && mod == Qt::NoModifier)
			{
				addNewItem();
				return true;
			}
			else if (key == Qt::Key_Up   && mod == Qt::ControlModifier)
			{
				moveUpSelected();
				return true;
			}
			else if (key == Qt::Key_Down && mod == Qt::ControlModifier)
			{
				moveDownSelected();
				return true;
			}
		}
	}

	return QWidget::eventFilter(a_watched, a_event);
}

void LibraryWidget::onAdd()
{
	addNewItem();
}

void LibraryWidget::onDelete()
{
	deleteSelectedItem();
}

void LibraryWidget::onContextMenuRequested(const QPoint& a_position)
{
	const QModelIndex selected_index = m_ui->lstItems->indexAt(a_position);
	if (!selected_index.isValid())
		return;

	QMenu menu;
	const auto act_set_color = menu.addAction("Set &color");
	const auto act_rename    = menu.addAction("&Rename");
	const auto act_delete    = menu.addAction("&Delete");

	const QPoint screen_position = m_ui->lstItems->mapToGlobal(a_position);
	auto selected = menu.exec( screen_position );

	if (selected == act_set_color)
	{
		QModelIndex color_index = selected_index.siblingAtColumn(LibraryModel::Column::Color);
		m_ui->lstItems->edit( color_index );
	}
	else if (selected == act_rename)
	{
		QModelIndex name_index  = selected_index.siblingAtColumn(LibraryModel::Column::Color);
		m_ui->lstItems->edit( name_index );
	}
	else if (selected == act_delete)
		m_model->removeRow(selected_index.row());
}

void LibraryWidget::onSelectionChanged(const QItemSelection& a_selected, const QItemSelection& a_deselected)
{
	Q_UNUSED(a_deselected);
	Q_ASSERT(a_selected.count() < 2);

	if (a_selected.size() == 0)
		emit instrumentSelected( WrongInstrumentIndex );
	else
	{
		QModelIndex top_left = a_selected.begin()->topLeft();
		QModelIndex script_index = top_left.siblingAtColumn(LibraryModel::Column::Script);
		emit instrumentSelected( script_index.row() );
	}
}

void LibraryWidget::addNewItem()
{
	m_model->append();
}

void LibraryWidget::deleteSelectedItem()
{
	const QModelIndex current = m_ui->lstItems->currentIndex();
	if (!current.isValid())
		return;

	int row = current.row();
	m_model->removeRow(row);

	if (m_model->rowCount() == 0)
		m_ui->lstItems->setCurrentIndex( QModelIndex() );
	else
	{
		if (row == m_model->rowCount())
			row--;
		QModelIndex new_current = m_model->index(row, 0);
		m_ui->lstItems->setCurrentIndex(new_current);
		onSelectionChanged({new_current,new_current}, QItemSelection());
	}
}

void LibraryWidget::moveUpSelected()
{
	const QModelIndex current = m_ui->lstItems->currentIndex();
	const int row_src = current.row();
	if (!current.isValid() ||
		 row_src == 0)
		return;

	const int row_dst = row_src-1;
	m_model->swap(row_src, row_dst);
	m_ui->lstItems->setCurrentIndex( current.model()->index(row_dst,0) );
}

void LibraryWidget::moveDownSelected()
{
	const QModelIndex current = m_ui->lstItems->currentIndex();
	const int row_src = current.row();
	if (!current.isValid())
		return;

	if (row_src > m_model->rowCount()-2)
		return;
	const int row_dst = row_src+1;
	m_model->swap(row_src, row_dst);
	m_ui->lstItems->setCurrentIndex( m_model->index(row_dst, 0) );
}

void LibraryWidget::updateDeleteButtonState()
{
	bool enabled = (m_model->rowCount() > 0);
	m_ui->btnDelete->setEnabled(enabled);
}
