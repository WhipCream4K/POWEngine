#include "pch.h"
#include "Core.h"

#include "POWEngine/Services/ServiceLocator.h"
#include "POWEngine/Window/Window.h"
#include "POWEngine/Services/ServiceLocator.h"

powe::Core::Core()
{
}

bool powe::Core::TranslateWindowInputs(const SharedPtr<Window>& window)
{
	bool isEarlyExit{};

	const WindowMessages messages{ window->PollWindowMessages(isEarlyExit) };

	// TODO: do the translation here

	return isEarlyExit;
}

bool powe::Core::TranslateWindowInputs(const Window& window)
{
	bool isEarlyExit{};

	window.PollWindowMessages(isEarlyExit);

	// TODO: if the window isn't focus then drop all the input receive

	return isEarlyExit;
}

void powe::Core::Step(const SharedPtr<WorldEntity>& worldEntt)
{
	UNREF_PARAM(worldEntt);
	// updates the world also create tasks for multi-thread rendering
}

powe::Core::~Core() = default;
