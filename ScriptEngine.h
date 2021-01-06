#pragma once
#include <QJSEngine>
#include <QGraphicsItem>
#include "ScriptKeyword.h"
#include "PatternItem.h"
#include "LibraryModel.h"
#include "SynthMode.h"

class ScriptEngine: public QObject
{
	Q_OBJECT
public:
	ScriptEngine();

	void init(const LibraryModelPtr& a_model);
	SynthMode run(const PatternItem* a_item, int a_frame);

signals:
	void scriptError(int a_index, const QString& a_error);

public slots:
	void onLibraryItemAdded  (const QModelIndex& a_parent, int a_start, int a_end);
	void onLibraryItemChanged(const QModelIndex& a_top_left, const QModelIndex& a_bottom_right);
	void onLibraryItemDeleted(const QModelIndex& a_parent, int a_start, int a_end);

	void onPatternItemAdded  (QGraphicsItem* a_item);
	void onPatternItemDeleted(QGraphicsItem* a_item);

private:
	void updateScriptObject(int a_index, const LibraryItem* a_item);

	LibraryModelPtr m_model;
	QMap<const LibraryItem*, QJSValue> m_script_objects;
	QMap<const PatternItem*, QJSValue> m_selfs;
	QJSEngine m_engine;
	QJSValue m_arg_object;
	QJSValue m_snd_object;
};

using ScriptEnginePtr = QSharedPointer<ScriptEngine>;