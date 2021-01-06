#pragma once
#include <type_traits>
#include <cstdint>
#include <limits>
#include <QFile>
#include "PatternPlayer.h"
#include "properties.h"

class SoundExporter
{
public:
	SoundExporter(PatternPlayer& a_player, const QString& a_file_name);
	
	bool exec();

private:
	template<class T>
	void output(const T& a_value)
	{
		static_assert(std::is_integral<T>());
		m_file.write( reinterpret_cast<const char*>(&a_value), sizeof(a_value) );
	}

	PatternPlayer& m_player;
	QFile m_file;
};