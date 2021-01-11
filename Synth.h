#pragma once
#include <qglobal.h>
#include <QSharedPointer>
#include "properties.h"
#include "SynthType.h"

class Synth
{
public:
	virtual ~Synth();

	double value(int a_index);
	virtual SynthType type() = 0;
	virtual void generate(double a_phase, double a_frequency) = 0;
protected:
	void setValue(int a_index, double a_value);
private:
	double m_values[Properties::Sound::SamplesPerFrame];
};

using SynthPtr = QSharedPointer<Synth>;