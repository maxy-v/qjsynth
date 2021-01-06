#pragma once
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "LibraryModel.h"
#include "PatternItem.h"
#include "PatternScene.h"
#include "QtTools.h"

class ProjectFile
{
public:
	static bool save(
		const QString& a_path,
		const LibraryModel* const a_library_model,
		const PatternScene* a_pattern_scene
	);
	static bool load(
		const QString& a_path,
		LibraryModel* const a_library_model,
		PatternScene* a_pattern_scene
	);

private:
	static int instrumentIndex(
		const PatternItem* a_item,
		const LibraryModel* const a_library_model);

	struct SingleAttribute
	{
		bool    valid = false;
		QString value;
	};
	static const SingleAttribute loadSingleAttribute(const QXmlStreamAttributes& a_attributes);
	struct InstrumentAttributes
	{
		bool    valid = false;
		QString name;
		QString script;
		QColor  color;
	};
	static const InstrumentAttributes loadInstrumentAttributes(const QXmlStreamAttributes& a_attributes);
	struct PatternItemAttributes
	{
		bool  valid = false;
		Key   key;
		Frame frame;
		Frame size;
		int   instrument_index;
	};
	static const PatternItemAttributes loadPatternItemAttributes(
		const QXmlStreamAttributes& a_attributes,
		const LibraryModel* const a_library_model
	);

	static constexpr const char* ElementProject           = "qjsynth_project";
	static constexpr const char* ElementPatternSize       = "pattern_size";
	static constexpr const char* AttributeValue           = "value";
	static constexpr const char* ElementSynth             = "synth";
	static constexpr const char* AttributeName            = "name";
	static constexpr const char* AttributeScript          = "script";
	static constexpr const char* AttributeColor           = "color";
	static constexpr const char* ElementPatternItem       = "pitem";
	static constexpr const char* AttributeKey             = "key";
	static constexpr const char* AttributeFrame           = "frame";
	static constexpr const char* AttributeSize            = "size";
	static constexpr const char* AttributeInstrumentIndex = "index";
};