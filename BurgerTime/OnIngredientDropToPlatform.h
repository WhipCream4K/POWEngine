#pragma once

#include "Subject.h"

class OnIngredientDropToPlatform : public Subject
{

public:

	void SignalFinishedDrop(powe::WorldEntity&,int playerIndex);

};

