#include "Clipboard.h"

void Clipboard::init(const PatternScenePtr& a_scene)
{
	Q_ASSERT( m_scene.isNull()  && !a_scene.isNull()  );
	m_scene  = a_scene;
}

void Clipboard::copy()
{
	auto items = m_scene->selectedItems();
	if (items.size() == 0)
		return;

	reset();

	int smallest_frame = items[0]->frame();
	for (auto item: items)
	{
		if (smallest_frame > item->frame())
			smallest_frame = item->frame();
	}

	for (auto item: items)
	{
		m_items << ClipboardItem{
			item->key(),
			item->frame() - smallest_frame,
			item->size(),
			item->libraryItem().toWeakRef()
		};

	}
}

void Clipboard::reset()
{
	m_items.clear();
}

bool Clipboard::isValid() const
{
	for (auto& item: m_items)
	{
		if (item.libraryItem().isNull())
			return false;
	}
	return true;
}

int Clipboard::count() const
{
	return m_items.count();
}

const ClipboardItem& Clipboard::item(int a_index) const
{
	Q_ASSERT(a_index >= 0 && a_index < m_items.count());
	return m_items[a_index];
}