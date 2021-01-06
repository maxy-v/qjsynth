#include "LibraryItem.h"

LibraryItem::LibraryItem(
	const QString& a_name,
	const QString& a_script,
	const QColor&  a_color)
	: m_name  (a_name)
	, m_script(a_script)
	, m_color (a_color)
{
}

const QString& LibraryItem::name()   const { return m_name; }
const QString& LibraryItem::script() const { return m_script; }
const QColor&  LibraryItem::color()  const { return m_color; }

void LibraryItem::setName(const QString& a_name)
{
	m_name = a_name;
}

void LibraryItem::setScript(const QString& a_script)
{
	m_script = a_script;
}

void LibraryItem::setColor(const QColor& a_color)
{
	m_color = a_color;
}