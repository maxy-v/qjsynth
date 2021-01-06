#include "SynthMode.h"

SynthType SynthMode::type() const
{
	return m_type;
}

float SynthMode::volumeFrom() const
{
	return m_volume_from;
}

float SynthMode::volumeTo() const
{
	return m_volume_to;
}

float SynthMode::pitch() const
{
	return m_pitch;
}

bool SynthMode::isDutySet() const
{
	return (m_duty >= MinDuty && m_duty < MaxDuty);
}

float SynthMode::duty() const
{
	return m_duty;
}

void SynthMode::setType(const SynthType& a_type)
{
	if (SynthType::isValid(a_type))
		m_type = a_type;
}

void SynthMode::setVolumeFrom(float a_volume)
{
	if (a_volume >= MinVolume && a_volume <= MaxVolume)
		m_volume_from = a_volume;
}

void SynthMode::setVolumeTo(float a_volume)
{
	if (a_volume >= MinVolume && a_volume <= MaxVolume)
		m_volume_to = a_volume;
}

void SynthMode::setPitch(float a_pitch)
{
	if (a_pitch >= MinPitch || a_pitch <= MaxPitch)
		m_pitch = a_pitch;
}

void SynthMode::setDuty(float a_duty)
{
	if (a_duty >= MinDuty && a_duty <= MaxDuty)
		m_duty = a_duty;
}