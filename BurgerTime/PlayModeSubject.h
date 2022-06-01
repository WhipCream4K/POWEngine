#pragma once

#include "StaticVariables.h"
#include "Subject.h"


class PlayModeSubject : public Subject
{
public:

	~PlayModeSubject() override = default;

	PlayMode PlayMode{ PlayMode::SinglePlayer };
};

