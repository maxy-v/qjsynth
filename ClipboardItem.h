#pragma once
#include "core_types.h"
#include "LibraryItem.h"

class ClipboardItem
{
public:
	ClipboardItem() = delete;
	ClipboardItem(Key a_key, Frame a_frame, Frame a_size, LibraryItemWCPtr a_library_item);

	Key   key() const;
	Frame frame() const;
	Frame size() const;
	const LibraryItemWCPtr& libraryItem() const;

private:
	Key   m_key;
	Frame m_frame;
	Frame m_size;
	LibraryItemWCPtr m_library_item;
};