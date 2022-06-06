#pragma once

#include "Subject.h"

class OnIngredientStepped : public Subject
{
public:
	void SignalStep(powe::WorldEntity&);
};

