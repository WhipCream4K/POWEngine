#include "pch.h"
#include "Core.h"

//#include "Input/InputTypes.h"
//#include "Input/Key.h"
#include "POWEngine/Services/ServiceLocator.h"
#include "POWEngine/Window/Window.h"
#include "POWEngine/Core/Clock/WorldClock.h"
#include "POWEngine/Logger/LoggerUtils.h"
#include "POWEngine/Rendering/RenderAPI.h"
#include "POWEngine/Rendering/Renderer.h"
#include "WorldEntity/WorldEntity.h"


powe::Core::Core()
	: m_WorldClock(std::make_shared<WorldClock>())
	, m_MainRenderer(std::make_shared<Renderer>())
{
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

	//SFMLRenderer some{};

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

void powe::Core::Step(WorldEntity& worldEntity) const
{
	worldEntity.ResolveEntities();

	const float deltaTime{ m_WorldClock->GetDeltaTime() };

	worldEntity.UpdatePipeline(PipelineLayer::Async, deltaTime);
	worldEntity.UpdatePipeline(PipelineLayer::Update, deltaTime);
	worldEntity.UpdatePipeline(PipelineLayer::PostUpdate, deltaTime);

	//ServiceLocator::GetSoundSystem().Update();
}

void powe::Core::Draw(const SharedPtr<Window>& window, const SharedPtr<WorldEntity>& worldEntt) const
{
	window->ClearWindow();
	m_MainRenderer->UpdateSystem(worldEntt->GetActiveArchetypes());
	m_MainRenderer->Draw(*window);
	window->Display();
}

void powe::Core::Draw(const Window& window, const WorldEntity& worldEntt) const
{
	window.ClearWindow();
	m_MainRenderer->UpdateSystem(worldEntt.GetActiveArchetypes());
	m_MainRenderer->Draw(window);
	window.Display();
}

void powe::Core::RegisterRendererType(OwnedPtr<RenderAPI>&& renderAPI) const
{
	m_MainRenderer->RegisterRenderAPI(std::move(renderAPI));
}

powe::Core::~Core() = default;
