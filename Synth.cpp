#include "Synth.h"

Synth::~Synth()
{
}

double Synth::value(int a_index)
{
	Q_ASSERT(a_index >= 0 && a_index < sizeof(m_values));
	return m_values[a_index];
}

void Synth::setValue(int a_index, double a_value)
{
	Q_ASSERT(a_index >= 0 && a_index < sizeof(m_values));
	m_values[a_index] = a_value;
}