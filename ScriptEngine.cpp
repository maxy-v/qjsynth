#include "ScriptEngine.h"

ScriptEngine::ScriptEngine()
{
	m_snd_object = m_engine.newObject();
	m_arg_object = m_engine.newObject();
	m_engine.globalObject().setProperty(ScriptKeyword::Snd, m_snd_object);
	m_engine.globalObject().setProperty(ScriptKeyword::Arg, m_arg_object);
}

void ScriptEngine::init(const LibraryModelPtr& a_model)
{
	Q_ASSERT( m_model.isNull());
	Q_ASSERT(!a_model.isNull());
	m_model = a_model;
}

SynthMode ScriptEngine::run(const PatternItem* a_item, int a_frame)
{
	// Prepare self
	Q_ASSERT(m_selfs.count(a_item) == 1);
	const QJSValue& self = m_selfs[a_item];
	m_engine.globalObject().setProperty(ScriptKeyword::Self, self);
	m_arg_object.setProperty(ScriptKeyword::AgrFrame , a_frame);
	m_arg_object.setProperty(ScriptKeyword::AgrFrames, a_item->size());

	// Reset sound properties
	m_snd_object.deleteProperty(ScriptKeyword::SndType);
	m_snd_object.deleteProperty(ScriptKeyword::SndVolumeFrom);
	m_snd_object.deleteProperty(ScriptKeyword::SndVolumeTo);
	m_snd_object.deleteProperty(ScriptKeyword::SndPitch);
	m_snd_object.deleteProperty(ScriptKeyword::SndDuty);
	

	// Prepare constants
	m_snd_object.setProperty(ScriptKeyword::SndSin     , SynthType::Sin);
	m_snd_object.setProperty(ScriptKeyword::SndSquare  , SynthType::Square);
	m_snd_object.setProperty(ScriptKeyword::SndTriangle, SynthType::Triangle);
	m_snd_object.setProperty(ScriptKeyword::SndSaw     , SynthType::Saw);

	// Run script
	auto library_item = a_item->libraryItem().get();
	Q_ASSERT(m_script_objects.count(library_item) == 1);
	QJSValue& script = m_script_objects[library_item];
	SynthMode mode;
	if (script.isCallable())
	{
		auto result = script.call();
		if (!result.isError())
		{
			if (m_snd_object.hasProperty(ScriptKeyword::SndType))
			{
				SynthType type = m_snd_object.property(ScriptKeyword::SndType).toInt();
				mode.setType(type);
			}
			if (m_snd_object.hasProperty(ScriptKeyword::SndVolumeFrom))
			{
				float volume = m_snd_object.property(ScriptKeyword::SndVolumeFrom).toVariant().value<float>();
				mode.setVolumeFrom(volume);
			}
			if (m_snd_object.hasProperty(ScriptKeyword::SndVolumeTo))
			{
				float volume = m_snd_object.property(ScriptKeyword::SndVolumeTo).toVariant().value<float>();
				mode.setVolumeTo(volume);
			}
			if (m_snd_object.hasProperty(ScriptKeyword::SndPitch))
			{
				float pitch = m_snd_object.property(ScriptKeyword::SndPitch).toVariant().value<float>();
				mode.setPitch(pitch);
			}
			if (m_snd_object.hasProperty(ScriptKeyword::SndDuty))
			{
				float duty = m_snd_object.property(ScriptKeyword::SndDuty).toVariant().value<float>();
				mode.setDuty(duty);
			}
		}
	}
	return mode;
}

void ScriptEngine::onLibraryItemAdded(const QModelIndex& a_parent, int a_start, int a_end)
{
	Q_ASSERT(!a_parent.isValid());
	
	for (int index = a_start; index <= a_end; index++)
	{
		auto item = m_model->item(index).get();
		Q_ASSERT(m_script_objects.count(item) == 0);
		updateScriptObject(index, item);
	}
}

void ScriptEngine::onLibraryItemChanged(const QModelIndex& a_top_left, const QModelIndex& a_bottom_right)
{
	if (a_top_left.column()     > LibraryModel::Column::Script ||
		a_bottom_right.column() < LibraryModel::Column::Script )
		return;

	for (int index = a_top_left.row(); index <= a_bottom_right.row(); index++)
	{
		auto item = m_model->item(index).get();
		Q_ASSERT(m_script_objects.count(item) == 1);
		updateScriptObject(index, item);
	}
}

void ScriptEngine::onLibraryItemDeleted(const QModelIndex& a_parent, int a_start, int a_end)
{
	Q_ASSERT(!a_parent.isValid());
	
	for (int index = a_start; index <= a_end; index++)
	{
		auto item = m_model->item(index).get();
		Q_ASSERT(m_script_objects.count(item) == 1);
		m_script_objects.remove(item);
	}
}

void ScriptEngine::onPatternItemAdded(QGraphicsItem* a_item)
{
	PatternItem* item = dynamic_cast<PatternItem*>(a_item);
	if (item == nullptr)
		return;
	Q_ASSERT(m_selfs.count(item) == 0);
	m_selfs[item] = m_engine.newObject();
}

void ScriptEngine::onPatternItemDeleted(QGraphicsItem* a_item)
{
	PatternItem* item = dynamic_cast<PatternItem*>(a_item);
	if (item == nullptr)
		return;
	Q_ASSERT(m_selfs.count(item) == 1);
	m_selfs.remove(item);
}

void ScriptEngine::updateScriptObject(int a_index, const LibraryItem* a_item)
{
	QString script = QString("(function() { %1 })").arg( a_item->script() );
	QJSValue script_object = m_engine.evaluate(script);
	if (script_object.isError())
		emit scriptError(a_index, script_object.toString());
	if (!script_object.isCallable())
		script_object = QJSValue();
	m_script_objects[a_item] = script_object;
}