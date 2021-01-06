#pragma once
#include "SynthType.h"

class SynthMode
{
public:
	SynthType type() const;
	float     volumeFrom() const;
	float     volumeTo() const;
	float     pitch() const;
	bool      isDutySet() const;
	float     duty() const;

	void setType(const SynthType& a_type);
	void setVolumeFrom(float a_volume);
	void setVolumeTo(float a_volume);
	void setPitch(float a_pitch);
	void setDuty(float a_duty);

private:
	static constexpr float MinVolume = 0.0f;
	static constexpr float MaxVolume = 1.0f;
	static constexpr float MinPitch     = -10.0f;
	static constexpr float MaxPitch     = +10.0f;
	static constexpr float DefaultPitch =   0.0f;
	static constexpr float MinDuty = 0.01f;
	static constexpr float MaxDuty = 1.0f-MinDuty;
	static constexpr float WrongDuty = MinDuty-1.0f;

	SynthType m_type        = SynthType::Sin;
	float     m_volume_from = MaxVolume;
	float     m_volume_to   = MaxVolume;
	float     m_pitch       = DefaultPitch;
	float     m_duty        = WrongDuty;
};