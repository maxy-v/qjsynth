#pragma once

#define SCRIPT_KEYWORD_SELF "self"
#define SCRIPT_KEYWORD_ARG  "arg"
#define SCRIPT_KEYWORD_SND  "snd"

#define SCRIPT_KEYWORD_ARG_FRAME  "frame"
#define SCRIPT_KEYWORD_ARG_FRAMES "frames"

#define SCRIPT_KEYWORD_SND_TYPE        "type"
#define SCRIPT_KEYWORD_SND_VOLUME_FROM "volume_from"
#define SCRIPT_KEYWORD_SND_VOLUME_TO   "volume_to"
#define SCRIPT_KEYWORD_SND_PITCH       "pitch"
#define SCRIPT_KEYWORD_SND_DUTY        "duty"

#define SCRIPT_KEYWORD_SND_SIN      "SIN"
#define SCRIPT_KEYWORD_SND_SQUARE   "SQUARE"
#define SCRIPT_KEYWORD_SND_TRIANGLE "TRIANGLE"
#define SCRIPT_KEYWORD_SND_SAW      "SAW"

namespace ScriptKeyword
{
	constexpr const char* Self = SCRIPT_KEYWORD_SELF;
	constexpr const char* Arg  = SCRIPT_KEYWORD_ARG;
	constexpr const char* Snd  = SCRIPT_KEYWORD_SND;

	constexpr const char* AgrFrame  = SCRIPT_KEYWORD_ARG_FRAME;
	constexpr const char* AgrFrames = SCRIPT_KEYWORD_ARG_FRAMES;

	constexpr const char* SndType       = SCRIPT_KEYWORD_SND_TYPE;
	constexpr const char* SndVolumeFrom = SCRIPT_KEYWORD_SND_VOLUME_FROM;
	constexpr const char* SndVolumeTo   = SCRIPT_KEYWORD_SND_VOLUME_TO;
	constexpr const char* SndPitch      = SCRIPT_KEYWORD_SND_PITCH;
	constexpr const char* SndDuty       = SCRIPT_KEYWORD_SND_DUTY;

	constexpr const char* SndSin      = SCRIPT_KEYWORD_SND_SIN;
	constexpr const char* SndSquare   = SCRIPT_KEYWORD_SND_SQUARE;
	constexpr const char* SndTriangle = SCRIPT_KEYWORD_SND_TRIANGLE;
	constexpr const char* SndSaw      = SCRIPT_KEYWORD_SND_SAW;
}