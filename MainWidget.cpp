#include "MainWidget.h"
#include "ui_MainWidget.h"

static constexpr const char* ProjectFileExtension =                       ".qjsxml";
static constexpr const char* ProjectFileFilter    = "qjsynth xml project (*.qjsxml)";
static constexpr const char* FileOperationSuccess = "Success";
static constexpr const char* FileOperationFailure = "Failure";

MainWidget::MainWidget(QWidget* a_parent)
	: QMainWindow(a_parent)
	, m_ui(new Ui::MainWidget)
	, m_shortcut_copy(this)
	, m_library_model( LibraryModelPtr::create() )
	, m_pattern_scene( PatternScenePtr::create() )
	, m_script_engine( ScriptEnginePtr::create() )
	, m_clipboard    ( ClipboardPtr   ::create() )
{
	m_ui->setupUi(this);
	m_ui->statusbar->setVisible(false);
	m_ui->splMain->setStretchFactor(0,5);
	m_ui->splMain->setStretchFactor(1,2);
	m_ui->splLibrary->setStretchFactor(0,2);
	m_ui->splLibrary->setStretchFactor(1,3);
	updateWindowTitle();

	m_shortcut_copy .setKey( QKeySequence("Ctrl+C") );
	connect(&m_shortcut_copy , &QShortcut::activated, &*m_clipboard, &Clipboard::copy);

	m_ui->wgtLibrary     ->init( m_library_model );
	m_ui->wgtScriptEditor->init( m_library_model );
	m_ui->wgtPattern     ->init( m_library_model, m_pattern_scene, m_clipboard );
	m_script_engine      ->init( m_library_model );
	m_player              .init( m_pattern_scene, m_script_engine );
	m_clipboard          ->init( m_pattern_scene );

	connect(m_ui->actionNew      , &QAction::triggered, this, &MainWidget::newProject);
	connect(m_ui->actionOpen     , &QAction::triggered, this, &MainWidget::openProject);
	connect(m_ui->actionSave     , &QAction::triggered, [this]() { saveProject(m_project_file_name); });
	connect(m_ui->actionSaveAs   , &QAction::triggered, [this]() { saveProject(                   ); });
	connect(m_ui->actionWavExport, &QAction::triggered, this, &MainWidget::exportToWav);
	connect(m_ui->actionQuit     , &QAction::triggered, this, &QWidget::close);

	connect(m_ui->actionPlay , &QAction::triggered, this, &MainWidget::play);
	connect(m_ui->actionPause, &QAction::triggered, this, &MainWidget::pause);
	connect(m_ui->actionStop , &QAction::triggered, this, &MainWidget::stop);

	connect(m_ui->spnSize, QOverload<int>::of(&QSpinBox::valueChanged), m_ui->wgtPattern, &PatternWidget::onSizeChanged);

	connect(m_ui->wgtLibrary , &LibraryWidget::instrumentSelected       , m_ui->wgtScriptEditor, &ScriptEditWidget::onInstrumentSelected);
	connect(m_ui->wgtLibrary , &LibraryWidget::instrumentSelected       , m_ui->wgtPattern     , &PatternWidget   ::onInstrumentSelected);
	connect(&*m_library_model, &QAbstractItemModel::rowsAboutToBeRemoved, m_ui->wgtPattern     , &PatternWidget   ::onInstrumentDeleted);
	connect(&*m_library_model, &QAbstractItemModel::dataChanged         , m_ui->wgtPattern     , &PatternWidget   ::onInstrumentChanged);
	
	connect(m_ui->wgtScriptEditor, &ScriptEditWidget::editingStarted , [this]() { if (m_ui) m_ui->wgtLibrary->setEnabled(false); } );
	connect(m_ui->wgtScriptEditor, &ScriptEditWidget::editingFinished, [this]() { if (m_ui) m_ui->wgtLibrary->setEnabled(true ); } );
	connect(&*m_script_engine, &ScriptEngine::scriptError, m_ui->wgtScriptEditor, &ScriptEditWidget::onScriptError);
	
	connect(&*m_library_model, &QAbstractItemModel::rowsInserted        , &*m_script_engine, &ScriptEngine::onLibraryItemAdded);
	connect(&*m_library_model, &QAbstractItemModel::dataChanged         , &*m_script_engine, &ScriptEngine::onLibraryItemChanged);
	connect(&*m_library_model, &QAbstractItemModel::rowsAboutToBeRemoved, &*m_script_engine, &ScriptEngine::onLibraryItemDeleted);
	
	connect(&*m_pattern_scene, &PatternScene::itemAdded  , &*m_script_engine, &ScriptEngine::onPatternItemAdded);
	connect(&*m_pattern_scene, &PatternScene::itemDeleted, &*m_script_engine, &ScriptEngine::onPatternItemDeleted);
	
	connect(&m_sound_stream, &SoundStream::dataRequired, &m_player, &PatternPlayer::generateSound);

	connect(m_ui->wgtPattern, &PatternWidget::selectionChanged, &m_player, &PatternPlayer::onSelectionChanged);
}

MainWidget::~MainWidget()
{
	m_ui.reset();
}

void MainWidget::play()
{
	Q_ASSERT(m_ui->actionPlay->isEnabled());
	m_sound_stream.start(Properties::Sound::SampleRate, 1, Properties::Sound::BytesPerSample*8);
	m_ui->actionPlay ->setEnabled(false);
	m_ui->actionPause->setEnabled(true);
	m_ui->actionStop ->setEnabled(true);
}

void MainWidget::pause()
{
	Q_ASSERT(!m_ui->actionPlay->isEnabled());
	m_sound_stream.pause();
}

void MainWidget::stop()
{
	if (!m_ui->actionStop->isEnabled())
		return;

	m_sound_stream.stop();
	m_player.stop();
	m_ui->actionPlay ->setEnabled(true);
	m_ui->actionPause->setEnabled(false);
	m_ui->actionStop ->setEnabled(false);
}

void MainWidget::commonReset()
{
	stop();
	m_ui->wgtScriptEditor->reset();
	m_player.reset();
	m_clipboard.clear();
	updateWindowTitle();
}

void MainWidget::updateWindowTitle()
{
	QString name = m_project_file_name;
	if (name.isEmpty())
		name = QString("[New project #%1]").arg(++m_last_project_index);
	else
		name = name.mid( name.lastIndexOf(QRegularExpression("[\\\\/]"))+1 );
	setWindowTitle( QString("%1 - qjsynth by maxy_v").arg(name) );
}

void MainWidget::newProject()
{
	m_ui->wgtPattern->reset();
	m_pattern_scene->reset();
	m_ui->spnSize->reset();
	m_library_model->clear();
	m_project_file_name.clear();
	commonReset();
}

void MainWidget::openProject()
{
	QString file_name = QFileDialog::getOpenFileName(nullptr, "Open project", m_project_file_name, ProjectFileFilter);
	if (file_name.isEmpty())
		return;

	if (ProjectFile::load(
		file_name,
		m_library_model.get(),
		m_pattern_scene.get()
		))
	{
		m_ui->wgtPattern->resetSelection();
		QSignalBlocker blocker(m_ui->spnSize);
		m_ui->spnSize->setValue(m_pattern_scene->size());
		m_project_file_name = file_name;
		commonReset();
	}
	else
		QMessageBox::warning(nullptr, FileOperationFailure, "Loading failed");
}

void MainWidget::saveProject(const QString& a_path)
{
	const QString title = a_path.isEmpty() ? "Save project as" : "Save project";

	QString file_name = a_path;
	if (file_name.isEmpty())
		file_name = QFileDialog::getSaveFileName(nullptr, title, m_project_file_name, ProjectFileFilter);
	if (file_name.isEmpty())
		return;

	bool bSuccess = ProjectFile::save(
		file_name,
		m_library_model.get(),
		m_pattern_scene.get()
	);
	if (bSuccess)
	{
		m_project_file_name = file_name;
		updateWindowTitle();
		QMessageBox::information(nullptr, FileOperationSuccess, "Saved successfully");
	}
	else
		QMessageBox::warning    (nullptr, FileOperationFailure, "Saving failed");
}

void MainWidget::exportToWav()
{
	QString file_name = m_project_file_name;
	QString project_file_ext(ProjectFileExtension);
	if (file_name.endsWith(project_file_ext, Qt::CaseInsensitive))
		file_name.chop    (project_file_ext.size());
	file_name += ".wav";

	file_name = QFileDialog::getSaveFileName(nullptr, "WAV export", file_name, "WAV (*.WAV)");
	if (file_name.isEmpty())
		return;

	stop();
	SoundExporter exporter(m_player, file_name);
	if (exporter.exec())
		QMessageBox::information(nullptr, FileOperationSuccess, "Exported successfully");
	else
		QMessageBox::warning    (nullptr, FileOperationFailure, "Exporting failed");
}
