#pragma once
#include <QGraphicsItem>
#include <QPainter>
#include <QSharedPointer>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include "properties.h"
#include "PatternStyle.h"
#include "LibraryItem.h"

class PatternItem final: public QGraphicsItem
{
public:
	explicit PatternItem(const QPoint& a_point, const LibraryItemCPtr& a_library_item);
	explicit PatternItem(Key a_key, Frame a_frame, Frame a_size, const LibraryItemCPtr& a_library_item);

	LibraryItemCPtr libraryItem() const;
	bool isSameType(const LibraryItemCPtr& a_library_item) const;
	void setWidth(Pixel a_width);
	void setSize(Frame a_size);

	Frame frame() const;
	Key   key() const;
	Frame size() const;

	// QGraphicsItem
	virtual QRectF 	 boundingRect() const override;
	virtual void 	 paint(QPainter* a_painter, const QStyleOptionGraphicsItem* a_option, QWidget* a_widget = nullptr) override;
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange a_change, const QVariant& a_value) override;

private:
	void commonSetup();
	QPoint currentPosition();
	QPoint snappedPosition(const QPoint& a_point);

	LibraryItemCPtr m_library_item;

	Frame m_frame;
	Key   m_key;
	Frame m_size = Properties::Pattern::DefaultItemSize;
};