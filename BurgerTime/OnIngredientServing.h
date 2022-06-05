#pragma once

#include "Subject.h"

/**
 * \brief Signal when all the ingredients are on the plate
 */
class OnIngredientServing : public Subject
{
public:
	void SignalServing(powe::WorldEntity& worldEntity);
};

