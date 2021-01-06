#pragma once
#include <QWidget>
#include <QScopedPointer>
#include "LibraryModel.h"
#include "PatternScene.h"
#include "PatternItem.h"
#include "ValueSlider/ValueSlider.h"
#include "SignalerGraphicsView/ResizeMode.h"
#include "PatternSelection.h"
#include "Clipboard.h"

namespace Ui { class PatternWidget; }

class PatternWidget: public QWidget
{
	Q_OBJECT
public:
	explicit PatternWidget(QWidget* a_parent = nullptr);

	void init(const LibraryModelPtr& a_model, const PatternScenePtr& a_scene, const ClipboardPtr& a_clipboard);
	void reset();
	void resetSelection();

signals:
	void selectionChanged(const PatternSelection& a_selection);

public slots:
	void onSizeChanged(int a_size);

	void onInstrumentSelected(int a_index);
	void onInstrumentDeleted (const QModelIndex& a_parent, int a_first, int a_last);
	void onInstrumentChanged (const QModelIndex& a_top_left, const QModelIndex& a_bottom_right, const QVector<int>& a_roles);

	void onZoomIn();
	void onZoomOut();
	void onCreatePatternItem(const QPointF& a_position);
	void onCopyPatternItem  (const QPointF& a_position);
	void onPastePatternItems(const QPointF& a_position);
	void onDeletePatternItem(QGraphicsItem* a_item);
	void onResizePatternItem(QGraphicsItem* a_item, sgv::ResizeMode a_mode, const QRectF& a_initial_size, QPointF a_size_delta);

private:
	QScopedPointer<Ui::PatternWidget> m_ui;

	PatternScenePtr m_scene;
	LibraryModelPtr m_library_model;
	ClipboardPtr    m_clipboard;
	int m_index = WrongInstrumentIndex;
	ValueSlider<float> m_scales{0.01f, 0.02f, 0.05f, 0.1f, 0.2f, 0.5f, 1.0f};
};
