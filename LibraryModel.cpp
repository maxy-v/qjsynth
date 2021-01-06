#include "LibraryModel.h"

LibraryModel& LibraryModel::operator=(LibraryModel&& a_source) noexcept
{
	clear();
	
	m_items = std::move(a_source.m_items);
	m_last_id = a_source.m_last_id;

	if (m_items.size() > 0)
	{
		beginInsertRows(QModelIndex(), 0, m_items.size()-1);
		endInsertRows();
	}
	
	return *this;
}

void LibraryModel::append(const QString& a_name, const QString& a_script, const QColor& a_color)
{
	const int new_index = m_items.size();
	beginInsertRows(QModelIndex(), new_index, new_index);
	QString name = a_name;
	if (name == QString())
		name = QString("Synth #%1").arg(++m_last_id);
	m_items.push_back( LibraryItemPtr::create(name, a_script, a_color) );
	endInsertRows();
}

void LibraryModel::swap(int a_index1, int a_index2)
{
	Q_ASSERT(a_index1 >= 0 && a_index1 < rowCount());
	Q_ASSERT(a_index2 >= 0 && a_index2 < rowCount());
	Q_ASSERT(abs(a_index1-a_index2) == 1);

	if (a_index1 > a_index2)
		beginMoveRows(QModelIndex(), a_index1, a_index1, QModelIndex(), a_index2);
	else
		beginMoveRows(QModelIndex(), a_index2, a_index2, QModelIndex(), a_index1);
	m_items.swapItemsAt(a_index1, a_index2);
	endMoveRows();
}

void LibraryModel::clear()
{
	if (m_items.size() == 0)
		return;
	beginRemoveRows(QModelIndex(), 0, m_items.size()-1);
	m_items.clear();
	endRemoveRows();
}

LibraryItemPtr LibraryModel::item(int a_index) const
{
	Q_ASSERT(a_index >= 0 && a_index < m_items.size());
	return m_items[a_index];
}

int LibraryModel::columnCount(const QModelIndex& a_parent) const
{
	Q_UNUSED(a_parent);
	return static_cast<int>( Column::count() );
}

QVariant LibraryModel::data(const QModelIndex& a_index, int a_role) const
{
	Q_ASSERT(a_index.isValid());
	Q_ASSERT(a_index.row() < m_items.size());
	Q_ASSERT(a_index.column() < static_cast<int>(Column::count()));

	const LibraryItem* const item = m_items[a_index.row()].get();

	const bool display_or_edit_role =
		a_role == Qt::DisplayRole ||
		a_role == Qt::EditRole    ;

	switch(a_index.column())
	{
	case Column::Name:
		if (display_or_edit_role)
			return item->name();
		break;
	case Column::Script:
		if (display_or_edit_role)
			return item->script();
		break;
	case Column::Color:
		if (a_role == Qt::BackgroundRole)
			return item->color();
		break;
	}

	return QVariant();
}

Qt::ItemFlags LibraryModel::flags(const QModelIndex& a_index) const
{
	Q_UNUSED(a_index);

	Qt::ItemFlags flags = 
		Qt::ItemIsSelectable     |
		Qt::ItemIsEditable       |
		Qt::ItemIsDropEnabled    |
		Qt::ItemIsEnabled        |
		Qt::ItemNeverHasChildren ;

	if (a_index.isValid())
		flags |= Qt::ItemIsDragEnabled;

	return flags;
}

QModelIndex LibraryModel::index(int a_row, int a_column, const QModelIndex& a_parent) const
{
	Q_ASSERT(!a_parent.isValid());
	return createIndex(a_row, a_column);
}

QModelIndex LibraryModel::parent(const QModelIndex& a_index) const
{
	Q_UNUSED(a_index);
	return QModelIndex();
}

int LibraryModel::rowCount(const QModelIndex& a_parent) const
{
	if (a_parent.isValid())
		return 0;
	else
		return m_items.size();
}

bool LibraryModel::setData(const QModelIndex& a_index, const QVariant& a_value, int a_role)
{
	LibraryItem* const item = m_items[a_index.row()].get();

	switch(a_index.column())
	{
	case Column::Name:
		if (a_role != Qt::EditRole)
			return false;
		item->setName( a_value.toString() );
		break;
	case Column::Script:
		if (a_role != Qt::EditRole)
			return false;
		item->setScript( a_value.toString() );
		break;
	case Column::Color:
		if (a_role != Qt::BackgroundRole)
			return false;
		item->setColor( a_value.value<QColor>() );
		break;
	default:
		return false;
	}

	emit dataChanged(a_index, a_index, {a_role});
	return true;
}

bool LibraryModel::removeRows(int a_row, int a_column, const QModelIndex& a_parent)
{
	Q_UNUSED(a_column);

	beginRemoveRows(a_parent, a_row, a_row);
	m_items.removeAt(a_row);
	endRemoveRows();

	return true;
}

Qt::DropActions LibraryModel::supportedDropActions() const
{
	return Qt::MoveAction;
}

QStringList LibraryModel::mimeTypes() const
{
	return QStringList("text/plain");
}

QMimeData* LibraryModel::mimeData(const QModelIndexList& a_indexes) const
{
	Q_ASSERT(a_indexes.size() == 1);
	const QModelIndex index = a_indexes.back();

	if (columnCount() > 1) // view won't automatically remove dragged row in case on two or more columns
		m_dragged_row = index.row();

	QMimeData* data = new QMimeData();
	data->setText( QString::number(index.row()) );
	return data;
}

bool LibraryModel::dropMimeData(const QMimeData* a_data, Qt::DropAction a_action, int a_row, int a_column, const QModelIndex& a_parent)
{
	Q_UNUSED(a_column);
	Q_ASSERT(a_action == Qt::MoveAction);

	int row = m_items.size();
	if      (a_parent.isValid())
		row = a_parent.row();
	else if (a_row >= 0)
		row = a_row;

	beginInsertRows(QModelIndex(), row, row);
	{
		int source_index = a_data->text().toInt();
		Q_ASSERT(source_index >= 0 && source_index < m_items.size());
		auto  item   = m_items.at(source_index);
		auto& name   = item->name();
		auto& script = item->script();
		auto& color  = item->color();
		m_items.insert(row, LibraryItemPtr::create(name, script, color));
	}
	endInsertRows();

	if (m_dragged_row != UndefinedRow)
	{
		if (m_dragged_row >= row)
			m_dragged_row++;
		removeRow(m_dragged_row);
		m_dragged_row = UndefinedRow;
	}

	return true;
}
