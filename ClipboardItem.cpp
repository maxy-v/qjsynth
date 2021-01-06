#include "ClipboardItem.h"

ClipboardItem::ClipboardItem(Key a_key, Frame a_frame, Frame a_size, LibraryItemWCPtr a_library_item)
	: m_key(a_key)
	, m_frame(a_frame)
	, m_size(a_size)
	, m_library_item(a_library_item)
{
}

Key ClipboardItem::key() const
{
	return m_key;
}

Frame ClipboardItem::frame() const
{
	return m_frame;
}

Frame ClipboardItem::size() const
{
	return m_size;
}

const LibraryItemWCPtr& ClipboardItem::libraryItem() const
{
	return m_library_item;
}