#include "pch.h"
#include "Scene.h"

#include "ECS/ECSManager.h"

powe::Scene::Scene(Game& game)
	: m_Game(game)
	, m_InputManager(*this)
{
}

void powe::Scene::Start()
{
	for (auto& sceneSystem : m_SceneSystems)
	{
		sceneSystem->OnStart(*this);
	}
}

void powe::Scene::Exit()
{
	for (auto& sceneSystem : m_SceneSystems)
	{
		sceneSystem->OnExit(*this);
	}
}

void powe::Scene::Update(float deltaTime)
{
	for (auto& sceneSystem : m_SceneSystems)
	{
		sceneSystem->OnUpdate(*this, deltaTime);
	}
}
