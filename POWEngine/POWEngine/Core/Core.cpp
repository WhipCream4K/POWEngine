#include "pch.h"
#include "Core.h"

#include "../Window/Window.h"

bool powe::Core::TranslateWindowInputs(const SharedPtr<Window>& window)
{
	bool isEarlyExit{};

	window->PollWindowMessages(isEarlyExit);

	// TODO: do the translation here

	return isEarlyExit;
}
