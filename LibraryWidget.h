#pragma once
#include <QWidget>
#include <QMenu>
#include <QKeyEvent>
#include <QItemSelection>
#include <QScopedPointer>
#include "LibraryDelegate.h"
#include "LibraryModel.h"

namespace Ui { class LibraryWidget; }

class LibraryWidget: public QWidget
{
	Q_OBJECT
public:
	explicit LibraryWidget(QWidget* a_parent = nullptr);

	void init(const LibraryModelPtr& a_model);

signals:
	void instrumentSelected(int a_index) const;

protected:
	virtual bool eventFilter(QObject* a_watched, QEvent* a_event) override;

private slots:
	void onAdd();
	void onDelete();

	void onContextMenuRequested(const QPoint& a_position);
	void onSelectionChanged(const QItemSelection& a_selected, const QItemSelection& a_deselected);

private:
	void addNewItem();
	void deleteSelectedItem();
	void moveUpSelected();
	void moveDownSelected();
	void updateDeleteButtonState();

	QScopedPointer<Ui::LibraryWidget> m_ui;

	LibraryDelegate m_delegate;
	LibraryModelPtr m_model;
};