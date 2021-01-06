#pragma once
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QScopedPointer>
#include <QShortcut>
#include "properties.h"
#include "LibraryModel.h"
#include "ProjectFile.h"
#include "PatternScene.h"
#include "ScriptEngine.h"
#include "SoundStream/SoundStream.h"
#include "PatternPlayer.h"
#include "SoundExporter.h"
#include "Clipboard.h"

namespace Ui { class MainWidget; }

class MainWidget: public QMainWindow
{
	Q_OBJECT
public:
	explicit MainWidget(QWidget* a_parent = nullptr);
	~MainWidget();

private slots:
	void play();
	void pause();
	void stop();

private:
	void commonReset();
	void updateWindowTitle();
	void newProject();
	void openProject();
	void saveProject(const QString& a_path = QString());
	void exportToWav();

	QScopedPointer<Ui::MainWidget> m_ui;
	QShortcut m_shortcut_copy;

	LibraryModelPtr m_library_model;
	PatternScenePtr m_pattern_scene;
	ScriptEnginePtr m_script_engine;
	SoundStream     m_sound_stream;
	PatternPlayer   m_player;
	ClipboardPtr    m_clipboard;
	
	int m_last_project_index = 0;
	QString m_project_file_name;
};
