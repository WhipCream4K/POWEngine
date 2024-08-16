#include "pch.h"
#include "Scene.h"

#include "ECS/ECSManager.h"
#include "Utils/Utils.h"

powe::Scene::Scene(PMRResource* memResource)
	: m_ECSManager{AllocateUnique<ECSManager>(memResource, memResource)}
	  , m_InputManager{AllocateUnique<InputManager>(memResource, memResource)}
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
