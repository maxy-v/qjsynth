#pragma once
#include <QAbstractItemModel>
#include <QMimeData>
#include <QSharedPointer>
#include "LibraryItem.h"
#include "EnumClass/enum.h"
#include "PatternStyle.h"

constexpr int WrongInstrumentIndex = -1;

class LibraryModel: public QAbstractItemModel
{
	Q_OBJECT
public:
	LibraryModel() = default;
	LibraryModel(const LibraryModel& ) = delete;
	LibraryModel(      LibraryModel&&) = delete;
	LibraryModel& operator=(const LibraryModel& ) = delete;
	LibraryModel& operator=(      LibraryModel&& a_source) noexcept;

	// High level api
	void append(
		const QString& a_name   = QString(),
		const QString& a_script = QString(),
		const QColor&  a_color  = PatternStyle::DefaultItemColor
	);
	void swap(int a_index1, int a_index2);
	void clear();
	LibraryItemPtr item(int a_index) const;

	// QAbstractItemModel: basic implementation
	virtual int             columnCount(const QModelIndex& a_parent = QModelIndex()) const override;
	virtual QVariant        data(const QModelIndex& a_index, int a_role = Qt::DisplayRole) const override;
	virtual Qt::ItemFlags   flags(const QModelIndex& a_index) const override;
	virtual QModelIndex     index(int a_row, int a_column, const QModelIndex& a_parent = QModelIndex()) const override;
	virtual QModelIndex     parent(const QModelIndex& a_index) const override;
	virtual int             rowCount(const QModelIndex& a_parent = QModelIndex()) const override;
	virtual bool            setData(const QModelIndex& a_index, const QVariant& a_value, int a_role = Qt::EditRole) override;
	virtual bool            removeRows(int a_row, int a_column, const QModelIndex& a_parent) override;
	// QAbstractItemModel: drag&drop
	virtual Qt::DropActions supportedDropActions() const override;
	virtual QStringList     mimeTypes() const override;
	virtual QMimeData*      mimeData(const QModelIndexList& a_indexes) const override;
	virtual bool            dropMimeData(const QMimeData* a_data, Qt::DropAction a_action, int a_row, int a_column, const QModelIndex& a_parent) override;

	DECLARE_ENUM(Column, int, Name, Script, Color);

private:
	QList<LibraryItemPtr> m_items;
	int m_last_id{ 0 };

	static constexpr int UndefinedRow = -1;
	mutable int m_dragged_row = UndefinedRow;
};

using LibraryModelPtr = QSharedPointer<LibraryModel>;
