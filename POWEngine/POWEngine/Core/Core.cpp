#include "pch.h"
#include "Core.h"

#include "Input/InputManager.h"
#include "POWEngine/Window/Window.h"
#include "POWEngine/Core/Clock/WorldClock.h"
#include "POWEngine/Renderer/Renderer.h"
#include "WorldEntity/WorldEntity.h"
#include "POWEngine/Core/Input/InputStruct.h"

powe::Core::Core()
{
}

bool powe::Core::TranslateWindowInputs(const Window& window, WorldEntity& worldEntt) const
{
	m_WorldClock->Start();
	
	bool isEarlyExit{};
	bool ignoreInputs{};

	HardwareMessages hwMessages{};
	window.PollHardwareMessages(hwMessages, isEarlyExit, ignoreInputs);
	
	const float deltaTime{ m_WorldClock->GetDeltaTime() };
	// window.UpdateWindowContext(deltaTime);
	
	if (!ignoreInputs)
	{
		m_InputManager->PollHardWareMessages(hwMessages, deltaTime);
		worldEntt.GetInputSettings().ParseHWMessages(hwMessages);
	}
	
	return isEarlyExit;
}

void powe::Core::StartWorldClock() const
{
	m_WorldClock->ResetTime();
}

void powe::Core::Step(WorldEntity& worldEntity)
{
	m_WorldClock->Start();
	
	worldEntity.ResolveEntities();

	const float deltaTime{ m_WorldClock->GetDeltaTime() };
	
	worldEntity.UpdatePipeline(PipelineLayer::InputValidation, deltaTime);
	
	worldEntity.UpdatePipeline(PipelineLayer::Update, deltaTime);
	worldEntity.UpdatePipeline(PipelineLayer::PhysicsValidation, deltaTime);
	
	worldEntity.UpdatePipeline(PipelineLayer::PostUpdate, deltaTime);
}


bool powe::Core::FullStepMultiThreaded(const Renderer& renderer, WorldEntity& world)
{
	WaitForLastFrameDisplay(renderer);
	
	const Window* targetWindow{renderer.GetUnCheckedTargetWindow()};
	const bool shouldQuit = TranslateWindowInputs(*targetWindow,world);

	// Update
	{
		world.ResolveEntities();

		const float deltaTime{ m_WorldClock->GetDeltaTime() };

		world.UpdatePipeline(PipelineLayer::InputValidation, deltaTime);
	
		world.UpdatePipeline(PipelineLayer::Update, deltaTime);
		world.UpdatePipeline(PipelineLayer::PhysicsValidation, deltaTime);
	
		world.UpdatePipeline(PipelineLayer::PostUpdate, deltaTime);
	}

	// Render
	{
		renderer.ClearBackBuffer();
		world.RenderCommandPipeline(*targetWindow,*renderer.GetRenderAPI());
		renderer.DeferredDrawOnWindow();
	}

	// renderer.GetTargetWindow()->Display();
	
	m_WorldClock->End();

	if(!m_StartFrame && m_CurrentFrameCount++ == MaxQueuedFrame - 1)
		m_StartFrame = true;

	return shouldQuit;
}

bool powe::Core::FullStep(const Renderer& renderer, WorldEntity& world) const
{
	const Window* targetWindow{renderer.GetUnCheckedTargetWindow()};
	const bool shouldQuit = TranslateWindowInputs(*targetWindow,world);

	m_WorldClock->Start();

	
	// Update
	{
		world.ResolveEntities();

		const float deltaTime{ m_WorldClock->GetDeltaTime() };

		// Pre update on UI Thread
		renderer.Update(deltaTime);

		world.UpdatePipeline(PipelineLayer::InputValidation, deltaTime);
	
		world.UpdatePipeline(PipelineLayer::Update, deltaTime);
		world.UpdatePipeline(PipelineLayer::PhysicsValidation, deltaTime);
		
		world.UpdatePipeline(PipelineLayer::PostUpdate, deltaTime);
	}

	// Render
	{
		renderer.ClearBackBuffer();
		world.RenderCommandPipeline(*targetWindow,*renderer.GetRenderAPI());
		renderer.DisplayBuffer();
	}
	
	m_WorldClock->End();

	return shouldQuit;
}

powe::Core::~Core() = default;

void powe::Core::WaitForLastFrameDisplay(const Renderer& renderer) const
{
	if(m_StartFrame)
	{
		renderer.DisplayBuffer();
	}

	m_WorldClock->Start();
}
