#pragma once
#include <QWidget>
#include <QScopedPointer>
#include "LibraryModel.h"
#include "ScriptSyntaxHighlighter.h"
#include "QtTools.h"

namespace Ui { class ScriptEditWidget; }

class ScriptEditWidget: public QWidget
{
	Q_OBJECT
public:
	explicit ScriptEditWidget(QWidget* a_parent = nullptr);

	void init(const LibraryModelPtr& a_model);
	void reset();

signals:
	void editingStarted();
	void editingFinished();

public slots:
	void onInstrumentSelected(int a_index);
	void onScriptError(int a_index, const QString& a_error);

private slots:
	void onScriptTextChanged();

private:
	void hideErrorMessage();
	void endEditing(bool a_apply);
	bool isEditing() const;

	QScopedPointer<Ui::ScriptEditWidget> m_ui;
	ScriptSyntaxHighlighter m_highlighter;
	LibraryModelPtr m_model;
	int m_index;
};
