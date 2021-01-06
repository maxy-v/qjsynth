#include "SoundExporter.h"

SoundExporter::SoundExporter(PatternPlayer& a_player, const QString& a_file_name)
	: m_player(a_player)
{
	m_file.setFileName(a_file_name);
}

bool SoundExporter::exec()
{
	if (!m_file.open(QIODevice::WriteOnly))
		return false;

	m_player.stop();

	// WAV header

	m_file.write( "RIFF" );
	constexpr uint32_t HeaderSize = 44;
	const     uint32_t DataSize   = m_player.sampleCount()*Properties::Sound::BytesPerSample;
	uint32_t file_size = HeaderSize + DataSize - 8;
	output( file_size );
	m_file.write( "WAVE" );
	m_file.write( "fmt " );
	constexpr uint32_t ChunkSize = 16; // PCM
	output( ChunkSize );
	constexpr uint16_t Format = 1; // PCM
	output( Format );
	constexpr uint16_t Channels = 1;
	output( Channels );
	constexpr uint32_t SampleRate    = Properties::Sound::SampleRate;
	output( SampleRate );
	constexpr uint32_t ByteRate      = Properties::Sound::BytesPerSample * SampleRate;
	output( ByteRate );
	constexpr uint16_t BlockAlign    = Properties::Sound::BytesPerSample;
	output( BlockAlign );
	constexpr uint16_t BitsPerSample = Properties::Sound::BytesPerSample * 8;
	output( BitsPerSample );
	m_file.write( "data" );
	output( DataSize );

	// Audio samples

	uint32_t           data_left  = DataSize;
	constexpr uint32_t block_size = 1024;
	char               buffer[block_size];
	while (data_left > 0)
	{
		uint32_t size = block_size;
		if (size > data_left)
			size = data_left;

		m_player.generateSound(buffer, static_cast<qint32>(size));
		m_file.write(buffer, static_cast<qint32>(size));

		data_left -= size;
		Q_ASSERT(data_left >= 0);
	}

	return true;
}