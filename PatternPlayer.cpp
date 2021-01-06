#include "PatternPlayer.h"

void PatternPlayer::init(const PatternScenePtr& a_scene, ScriptEnginePtr& a_engine)
{
	Q_ASSERT( m_scene.isNull()  && !a_scene.isNull()  );
	Q_ASSERT( m_engine.isNull() && !a_engine.isNull() );
	m_scene  = a_scene;
	m_engine = a_engine;
}

int PatternPlayer::sampleCount() const
{
	PatternSelection selection = sizeLimitedSelection();
	const Frame frames = selection.isValid() ? selection.size()  : m_scene->size();
	return frames * Properties::Sound::SamplesPerFrame;
}

void PatternPlayer::stop()
{
	m_position = 0.0;
	m_states.clear();
}

void PatternPlayer::reset()
{
	stop();
	m_selection.invalidate();
}

void PatternPlayer::generateSound(char* a_buffer, qint64 a_size)
{
	// Prepare sound buffer

	std::fill_n(a_buffer, a_size, 0);
	Q_ASSERT(a_size%Properties::Sound::BytesPerSample == 0);
	a_size /= Properties::Sound::BytesPerSample;
	m_output = reinterpret_cast<Sample*>(a_buffer);
	Q_ASSERT(a_size < std::numeric_limits<int>::max() );
	Sample samples_to_write = static_cast<int>(a_size);

	// Fill sound buffer block by block

	const auto [count, first, last] = samplesToPlay();

	Q_ASSERT(count >= 0);
	if (count == 0)
		return;
	
	if (m_position > last)
		m_position = first;
	if (m_position < first)
		m_position = first;

	while (samples_to_write > 0)
	{
		const int samples_left = count-(m_position-first);
		Q_ASSERT(samples_left > 0);
		int block_samples = samples_to_write;
		if (block_samples > samples_left)
			block_samples = samples_left;

		processWindow(m_position, m_position + block_samples - 1);

		m_position += block_samples;
		Q_ASSERT(m_position <= last + 1);
		if (m_position == last + 1)
			m_position =  first;
		samples_to_write -= block_samples;
		m_output += block_samples;
		Q_ASSERT(samples_to_write >= 0);
	}
}

void PatternPlayer::onSelectionChanged(const PatternSelection& a_selection)
{
	m_selection = a_selection;
}

PatternSelection PatternPlayer::sizeLimitedSelection() const
{
	const Frame scene_last_frame = m_scene->size() - 1;
	Frame first = m_selection.first();
	Frame last  = m_selection.last();
	
	if (first > scene_last_frame &&
		last  > scene_last_frame)
		// Completely outside of the scene
		return PatternSelection();

	if (first > scene_last_frame)
		first = scene_last_frame;
	if (last  > scene_last_frame)
		last  = scene_last_frame;

	return PatternSelection(first, last);
}

std::tuple<int,int,int> PatternPlayer::samplesToPlay() const
{
	PatternSelection selection = sizeLimitedSelection();
	const int count = sampleCount();

	if (selection.isValid())
	{
		const int first = Properties::Sound::SamplesPerFrame* selection.first();
		return std::tuple<int,int,int>(count, first, first+count-1);
	}
	
	return std::tuple<int,int,int>(count, 0, count-1);
}

void PatternPlayer::processWindow(const int a_first_sample, const int a_last_sample)
{
	Q_ASSERT(a_first_sample <= a_last_sample);
	const int window_first_frame = a_first_sample/Properties::Sound::SamplesPerFrame;
	const int window_last_frame  = a_last_sample /Properties::Sound::SamplesPerFrame;
	auto items = m_scene->items(window_first_frame, window_last_frame);
	
	QSet<PatternItem*> unused_state_keys = QSet<PatternItem*>::fromList( m_states.keys() );

	// Precess each pattern item frame by frame

	for (PatternItem* item: items)
	{
		unused_state_keys.remove(item);

		int item_first_frame = item->frame();
		int item_last_frame  = item->frame() + item->size()-1;
		if (item_first_frame < window_first_frame)
			item_first_frame = window_first_frame;
		if (item_last_frame  > window_last_frame)
			item_last_frame  = window_last_frame;

		for (int frame = item_first_frame; frame <= item_last_frame; frame++)
		{
			States::Iterator state_iter = m_states.find(item);
			if (state_iter == m_states.end())
				state_iter  = m_states.insert(item, ItemState());
			ItemState& state = state_iter.value();

			SynthMode mode = m_engine->run(item, frame-item->frame());
			const double frequency = Properties::Piano::frequency(item->key()+mode.pitch());
			const bool same_frame = (state.frame == frame);

			if (!same_frame)
			{
				SynthPtr synth = SynthFactory::create(mode);
				if (!synth.isNull())
				{
					synth->generate(state.phase, frequency);
					for (int sample = 0; sample < Properties::Sound::SamplesPerFrame; sample++)
						state.data[sample] = synth->value(sample) * std::numeric_limits<Sample>::max();
				}
			}

			int frame_first_sample = (frame+0)*Properties::Sound::SamplesPerFrame;
			int frame_last_sample  = (frame+1)*Properties::Sound::SamplesPerFrame-1;
			if (frame_first_sample < a_first_sample)
				frame_first_sample = a_first_sample;
			if (frame_last_sample  > a_last_sample)
				frame_last_sample  = a_last_sample;

			for (int sample = frame_first_sample; sample <= frame_last_sample; sample++)
			{
				int output_sample_index = sample-a_first_sample;
				int input_sample_index  = sample%Properties::Sound::SamplesPerFrame;
				Q_ASSERT(output_sample_index >= 0);
				float volume = 
					mode.volumeFrom() + 
					input_sample_index*
						(mode.volumeTo()-mode.volumeFrom())
						/
						(Properties::Sound::SamplesPerFrame-1);
				volume *= Properties::Sound::MainVolume;
				float value = volume*state.data[input_sample_index];
				m_output[output_sample_index] += static_cast<Sample>(value);
			}

			if (!same_frame)
			{
				state.phase += frequency/Properties::Core::FramesPerSecond;
				state.phase  = fmod(state.phase, 1.0);
				state.frame = frame;
			}
		}
	}

	// Delete unused states

	for (auto key: unused_state_keys)
		m_states.remove(key);
}