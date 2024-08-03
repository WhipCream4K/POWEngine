#include "pch.h"
#include "Scene.h"

#include "ECS/ECSManager.h"
#include "Utils/Utils.h"

powe::Scene::Scene(EngineLayer* parent)
	: m_EngineLayer{ parent }
	, m_ECSManager{ AllocateUnique<ECSManager>(parent->GetAllocator(),parent->GetAllocator()) }
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

}

void powe::Scene::Update(float deltaTime)
{
	for (auto& sceneSystem : m_SceneSystems)
	{
		sceneSystem->OnUpdate(*this, deltaTime);
	}
}
