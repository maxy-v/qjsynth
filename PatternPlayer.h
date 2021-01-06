#pragma once
#include <limits>
#include <QSet>
#include <tuple>
#include "properties.h"
#include "PatternScene.h"
#include "PatternItem.h"
#include "ScriptEngine.h"
#include "SynthFactory.h"
#include "PatternSelection.h"

class PatternPlayer: public QObject
{
	Q_OBJECT
public:
	PatternPlayer() = default;

	void init(const PatternScenePtr& a_scene, ScriptEnginePtr& a_engine);

	int sampleCount() const;
	void stop();
	void reset();

public slots:
	void generateSound(char* a_buffer, qint64 a_size);
	void onSelectionChanged(const PatternSelection& a_selection);

private:
	PatternSelection sizeLimitedSelection() const;
	std::tuple<int,int,int> samplesToPlay() const;
	void processWindow(const int a_first_sample, const int a_last_sample);

	PatternScenePtr m_scene;
	ScriptEnginePtr m_engine;

	PatternSelection m_selection;
	int m_position = 0;
	Sample* m_output = nullptr;

	struct ItemState
	{
		static constexpr int UndefinedFrame = std::numeric_limits<Frame>::min();

		double phase = 0;
		Frame  frame = UndefinedFrame;
		Sample data[Properties::Sound::SamplesPerFrame];
	};
	using States = QMap<class PatternItem*, ItemState>;
	States m_states;
};