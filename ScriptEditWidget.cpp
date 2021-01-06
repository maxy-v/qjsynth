#include "ScriptEditWidget.h"
#include "ui_ScriptEditWidget.h"

ScriptEditWidget::ScriptEditWidget(QWidget* a_parent) :
	QWidget(a_parent),
	m_ui(new Ui::ScriptEditWidget)
{
	m_ui->setupUi(this);
	m_highlighter.setDocument(m_ui->edtScript->document());

	QtTools::setLayoutItemsEnabled(m_ui->layButtons, false);
	QtTools::setLayoutItemsVisible(m_ui->layButtons, false);
	
	m_ui->edtScript->setEnabled(false);
	QtTools::setTabSize(m_ui->edtScript, 4);
	connect(m_ui->edtScript, &QTextEdit::textChanged, this, &ScriptEditWidget::onScriptTextChanged);

	connect(m_ui->btnApply  , &QPushButton::clicked, [this]() { endEditing(true ); });
	connect(m_ui->btnDiscard, &QPushButton::clicked, [this]() { endEditing(false); });

	hideErrorMessage();
}

void ScriptEditWidget::init(const LibraryModelPtr& a_model)
{
	Q_ASSERT( m_model.isNull());
	Q_ASSERT(!a_model.isNull());
	m_model = a_model;
}

void ScriptEditWidget::reset()
{
	if (isEditing())
		endEditing(false);
	hideErrorMessage();
}

void ScriptEditWidget::onInstrumentSelected(int a_index)
{
	m_index = a_index;

	const bool selected = (m_index != WrongInstrumentIndex);
	QtTools::setLayoutItemsVisible(m_ui->layButtons, selected);
	QtTools::setLayoutItemsEnabled(m_ui->layButtons, selected);
	QSignalBlocker blocker(m_ui->edtScript);
	m_ui->edtScript->setEnabled(selected);
	m_ui->edtScript->clear();
	hideErrorMessage();
	
	if (!selected)
	{
		emit editingFinished();
		return;
	}
	
	QModelIndex index = m_model->index(m_index, LibraryModel::Column::Script);
	const QString script = index.data().value<QString>();
	m_ui->edtScript->setText(script);
}

void ScriptEditWidget::onScriptError(int a_index, const QString& a_error)
{
	if (m_index == a_index)
	{
		m_ui->lblError->setVisible(true);
		m_ui->lblError->setText(a_error);
	}
}

void ScriptEditWidget::onScriptTextChanged()
{
	if (isEditing())
		return;
	QtTools::setLayoutItemsEnabled(m_ui->layButtons, true);
	emit editingStarted();
	hideErrorMessage();
}

void ScriptEditWidget::hideErrorMessage()
{
	m_ui->lblError->clear();
	m_ui->lblError->setVisible(false);
}

void ScriptEditWidget::endEditing(bool a_apply)
{
	Q_ASSERT(m_index != WrongInstrumentIndex);
	QModelIndex index = m_model->index(m_index, LibraryModel::Column::Script);

	if (a_apply)
		m_model->setData(index, m_ui->edtScript->toPlainText());
	else
	{
		const QString& script = index.data().value<QString>();
		m_ui->edtScript->setText(script);
	}

	QtTools::setLayoutItemsEnabled(m_ui->layButtons, false);
	emit editingFinished();
}

bool ScriptEditWidget::isEditing() const
{
	return m_ui->btnApply->isEnabled();
}
