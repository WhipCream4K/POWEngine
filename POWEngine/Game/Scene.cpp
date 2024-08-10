#include "pch.h"
#include "Scene.h"

#include "ECS/ECSManager.h"
#include "Utils/Utils.h"

powe::Scene::Scene(EngineLayer* parent)
	: m_ECSManager{AllocateUnique<ECSManager>(parent->GetAllocator(), parent->GetAllocator())}
	  , m_InputManager{AllocateUnique<InputManager>(parent->GetAllocator(), parent->GetAllocator())}
	  , m_EngineLayer{*parent}
{
}

void powe::Scene::OnStart()
{
	for (auto& sceneSystem : m_SceneSystems)
	{
		sceneSystem->OnStart(*this);
	}
}

void powe::Scene::OnExit()
{
	for (auto& sceneSystem : m_SceneSystems)
	{
		sceneSystem->OnExit(*this);
	}
}

void powe::Scene::OnWindowEvents(const Window::EventQueue& winEvents) const
{
	m_InputManager->OnWindowEvents(winEvents);
}

void powe::Scene::Update(float deltaTime)
{
	for (auto& sceneSystem : m_SceneSystems)
	{
		sceneSystem->OnUpdate(*this, deltaTime);
	}
}
