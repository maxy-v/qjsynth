#pragma once
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QSharedPointer>
#include "properties.h"
#include "PatternItem.h"

class PatternScene: public QObject
{
	Q_OBJECT
public:
	PatternScene();
	PatternScene(const PatternScene& ) = delete;
	PatternScene(      PatternScene&&) = delete;
	PatternScene& operator=(const PatternScene& ) = delete;
	PatternScene& operator=(      PatternScene&&) = delete;

	QGraphicsScene* get();
	QList<PatternItem*> items() const;
	QList<PatternItem*> items(int a_first_frame, int a_last_frame) const;
	void deleteItem(PatternItem* a_item);
	void addItem   (PatternItem* a_item);
	void reset();
	Frame size() const;
	void setSize(Frame a_size);
	QList<PatternItem*> selectedItems() const;
	
signals:
	void itemAdded  (PatternItem* a_item);
	void itemDeleted(PatternItem* a_item);

private:
	void updateSceneRect();
	QList<PatternItem*> itemList(const QList<QGraphicsItem*>& a_items) const;

	QGraphicsScene m_scene;
	Frame m_size = Properties::Pattern::DefaultSize;
};

using PatternScenePtr = QSharedPointer<PatternScene>;