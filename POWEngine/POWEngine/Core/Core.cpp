#include "pch.h"
#include "Core.h"

//#include "Input/InputTypes.h"
#include "Input/Key.h"
#include "POWEngine/Services/ServiceLocator.h"
#include "POWEngine/Window/Window.h"
#include "POWEngine/Core/Clock/WorldClock.h"
#include "POWEngine/Logger/LoggerUtils.h"
#include "POWEngine/Rendering/Renderer.h"
#include "WorldEntity/WorldEntity.h"

powe::Core::Core()
	: m_WorldClock()
	, m_MainRenderer(std::make_shared<Renderer>())
{
	//ServiceLocator::Initialize();
}

bool powe::Core::TranslateWindowInputs(const SharedPtr<Window>& window, const SharedPtr<WorldEntity>& worldEntt) const
{
	m_WorldClock->Start();

	bool isEarlyExit{};
	bool ignoreInputs{};


	const HardwareMessages& hwMessages{ window->PollHardwareMessages(isEarlyExit,ignoreInputs) };

	if (!ignoreInputs)
	{
		worldEntt->GetInputSettings().ParseHWMessages(hwMessages);
	}

	return isEarlyExit;
}

bool powe::Core::TranslateWindowInputs(const Window& window, const SharedPtr<WorldEntity>& worldEntt) const
{
	m_WorldClock->Start();

	bool isEarlyExit{};
	bool ignoreInputs{};

	const HardwareMessages& hwMessages{ window.PollHardwareMessages(isEarlyExit,ignoreInputs) };

	if (!ignoreInputs)
	{
		worldEntt->GetInputSettings().ParseHWMessages(hwMessages);
	}

	return isEarlyExit;
}

bool powe::Core::TranslateWindowInputs(const Window& window, WorldEntity& worldEntt) const
{
	m_WorldClock->Start();

	bool isEarlyExit{};
	bool ignoreInputs{};

	const HardwareMessages& hwMessages{ window.PollHardwareMessages(isEarlyExit,ignoreInputs) };

	if (!ignoreInputs)
	{
		worldEntt.GetInputSettings().ParseHWMessages(hwMessages);
	}

	return isEarlyExit;
}

void powe::Core::StartWorldClock()
{
	m_WorldClock = std::make_shared<WorldClock>();
}

void powe::Core::Step(const SharedPtr<WorldEntity>&)
{
	// updates the world also create tasks for multi-thread rendering
}

void powe::Core::Draw(const SharedPtr<Window>& window, const SharedPtr<WorldEntity>& worldEntt) const
{
	m_MainRenderer->UpdateSystem(worldEntt->GetActiveArchetypes());
	m_MainRenderer->Draw(*window);
	window->ClearWindow();
}

void powe::Core::Draw(const Window& window, const WorldEntity& worldEntt) const
{
	m_MainRenderer->UpdateSystem(worldEntt.GetActiveArchetypes());
	m_MainRenderer->Draw(window);
	window.ClearWindow();
}

void powe::Core::RegisterRendererType(OwnedPtr<RenderAPI>&& renderAPI) const
{
	m_MainRenderer->RegisterRenderAPI(std::move(renderAPI));
}

powe::Core::~Core() = default;
