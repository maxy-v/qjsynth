#include "SynthFactory.h"

#include "SynthSin.h"
#include "SynthSquare.h"
#include "SynthTriangle.h"
#include "SynthSaw.h"

SynthPtr SynthFactory::create(const SynthMode& a_mode)
{
	switch(a_mode.type())
	{
	case SynthType::Sin:
		return SynthPtr(new SynthSin);
	case SynthType::Square:
		if (a_mode.isDutySet())
			return SynthPtr( new SynthSquare(a_mode.duty()) );
		else
			return SynthPtr( new SynthSquare );
	case SynthType::Triangle:
		return SynthPtr( new SynthTriangle );
	case SynthType::Saw:
		return SynthPtr( new SynthSaw );
	}

	return nullptr;
}
