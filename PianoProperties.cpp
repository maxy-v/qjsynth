#include "PianoProperties.h"

double Properties::Piano::frequency(double a_key)
{
	// https://en.wikipedia.org/wiki/Piano_key_frequencies
	return pow(2.0, (a_key-57)/12) * 440;
}

const char* Properties::Piano::keyName(Key a_key_index)
{
	if (a_key_index < 0)
		return "?";

	static const char* key_names[KeysInOctave] = {
		"c", "c#", "d", "d#", "e", "f", "f#", "g", "g#", "a", "a#", "b"
	};

	Octave octave = a_key_index/KeysInOctave;
	Key    key    = a_key_index%KeysInOctave;

	static char name[32];
	sprintf(name, "%s%d", key_names[key], octave);
	return name;
}
