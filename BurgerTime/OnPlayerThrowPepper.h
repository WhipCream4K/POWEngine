#pragma once

#include "Subject.h"

class OnPlayerThrowPepper : public Subject
{
public:

	OnPlayerThrowPepper() = default;
	void SignalThrowPepper(powe::WorldEntity&);
	
};

