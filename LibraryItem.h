#pragma once
#include <QString>
#include <QColor>
#include <QSharedPointer>

class LibraryItem
{
public:
	LibraryItem(
		const QString& a_name,
		const QString& a_script = QString(),
		const QColor&  a_color  = Qt::white);

	const QString& name() const;
	const QString& script() const;
	const QColor&  color() const;

	void setName  (const QString& a_name);
	void setScript(const QString& a_script);
	void setColor (const QColor&  a_color);

private:
	QString m_name;
	QString m_script;
	QColor  m_color{ Qt::white };
};

using LibraryItemPtr   = QSharedPointer<LibraryItem>;
using LibraryItemWCPtr = QWeakPointer<const LibraryItem>;
using LibraryItemCPtr  = QSharedPointer<const LibraryItem>;