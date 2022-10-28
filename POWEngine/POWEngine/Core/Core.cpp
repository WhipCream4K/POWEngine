#include "pch.h"
#include "Core.h"

#include "Input/InputManager.h"
#include "POWEngine/Window/Window.h"
#include "POWEngine/Core/Clock/WorldClock.h"
#include "POWEngine/Renderer/RenderAPI.h"
#include "POWEngine/Renderer/Renderer.h"
#include "WorldEntity/WorldEntity.h"
#include "POWEngine/Core/Input/InputStruct.h"


powe::Core::Core()
	: m_WorldClock(std::make_shared<WorldClock>())
	, m_MainRenderer(std::make_shared<Renderer>())
	, m_InputManager(std::make_unique<InputManager>())
{
}

bool powe::Core::TranslateWindowInputs(const SharedPtr<Window>& window, const SharedPtr<WorldEntity>& worldEntt) const
{
	m_WorldClock->Start();
	
	bool isEarlyExit{};
	bool ignoreInputs{};

	HardwareMessages hwMessages{};
	//const HardwareMessages& hwMessages{ window.PollHardwareMessages(isEarlyExit,ignoreInputs) };
	window->PollHardwareMessages(hwMessages, isEarlyExit, ignoreInputs);

	const float deltaTime{ m_WorldClock->GetDeltaTime() };
	window->UpdateWindowContext(deltaTime);
	
	if (!ignoreInputs)
	{
		m_InputManager->PollHardWareMessages(hwMessages, deltaTime);
		worldEntt->GetInputSettings().ParseHWMessages(hwMessages);
	}

	return isEarlyExit;
}

bool powe::Core::TranslateWindowInputs(const Window& window, WorldEntity& worldEntt) const
{
	m_WorldClock->Start();
	
	bool isEarlyExit{};
	bool ignoreInputs{};

	HardwareMessages hwMessages{};
	window.PollHardwareMessages(hwMessages, isEarlyExit, ignoreInputs);
	
	const float deltaTime{ m_WorldClock->GetDeltaTime() };
	window.UpdateWindowContext(deltaTime);
	
	if (!ignoreInputs)
	{
		m_InputManager->PollHardWareMessages(hwMessages, deltaTime);
		worldEntt.GetInputSettings().ParseHWMessages(hwMessages);
	}
	
	return isEarlyExit;
}

void powe::Core::StartWorldClock()
{
	m_WorldClock = std::make_shared<WorldClock>();
	m_WorldClock->ResetTime();
}

void powe::Core::Step(WorldEntity& worldEntity) const
{
	worldEntity.ResolveEntities();

	const float deltaTime{ m_WorldClock->GetDeltaTime() };

	worldEntity.UpdatePipeline(PipelineLayer::InputValidation, deltaTime);
	worldEntity.UpdatePipeline(PipelineLayer::Update, deltaTime);
	worldEntity.UpdatePipeline(PipelineLayer::PhysicsValidation, deltaTime);
	worldEntity.UpdatePipeline(PipelineLayer::PostUpdate, deltaTime);

}

void powe::Core::Draw(const SharedPtr<Window>& window, const SharedPtr<WorldEntity>& worldEntt) const
{
	window->ClearWindow();
	m_MainRenderer->UpdateSystem(*worldEntt,worldEntt->GetActiveArchetypes());
	m_MainRenderer->Draw(*window);
	window->Display();

	m_WorldClock->End();
}

void powe::Core::Draw(const Window& window, const WorldEntity& worldEntt) const
{
	window.ClearWindow();
	m_MainRenderer->UpdateSystem(worldEntt, worldEntt.GetActiveArchetypes());
	m_MainRenderer->Draw(window);
	window.Display();

	m_WorldClock->End();
}

void powe::Core::RegisterRendererType(OwnedPtr<RenderAPI>&& renderAPI) const
{
	m_MainRenderer->RegisterRenderAPI(std::move(renderAPI));
}

powe::Core::~Core() = default;
