#pragma once
#include <QObject>
#include <QSharedPointer>
#include "PatternScene.h"
#include "ClipboardItem.h"

class Clipboard: public QObject
{
	Q_OBJECT
public:
	void init(const PatternScenePtr& a_scene);

	void reset();
	bool isValid() const;
	int count() const;
	const ClipboardItem& item(int a_index) const;

public slots:
	void copy();

private:
	PatternScenePtr m_scene;
	QList<ClipboardItem> m_items;
};

using ClipboardPtr = QSharedPointer<Clipboard>;