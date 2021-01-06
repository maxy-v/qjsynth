#pragma once
#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QColorDialog>
#include "LibraryModel.h"
#include "QtTools.h"

class LibraryDelegate: public QStyledItemDelegate
{
	Q_OBJECT
public:
	explicit LibraryDelegate(QObject* a_parent = nullptr);

private slots:
	void onAccept();
	void onReject();

protected:
	virtual QWidget* createEditor(QWidget* a_parent, const QStyleOptionViewItem& a_option, const QModelIndex& a_index) const override;
	virtual void     setModelData(QWidget* a_editor, QAbstractItemModel* a_model, const QModelIndex& a_index) const override;
	virtual void     paint(QPainter* a_painter, const QStyleOptionViewItem& a_option, const QModelIndex& a_index) const override;
};
