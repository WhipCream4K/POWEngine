#include "SceneFactory.h"

#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

void SceneFactory::UnloadScene(powe::WorldEntity& worldEntity)
{
	for (const auto& gameObject : m_GameObjects)
	{
		worldEntity.RemoveGameObject(gameObject->GetID());
	}

	for (const auto& system : m_Systems)
	{
		worldEntity.RemoveSystem(system);
	}

	m_Systems.clear();
	m_GameObjects.clear();
}

void SceneFactory::AddGameObject(const SharedPtr<powe::GameObject>& gameObject)
{
	if (std::ranges::find(m_GameObjects, gameObject) == m_GameObjects.end())
		m_GameObjects.emplace_back(gameObject);
}

void SceneFactory::RemoveGameObject(const SharedPtr<powe::GameObject>& gameObject)
{
	const auto findItr{ std::ranges::find(m_GameObjects,gameObject) };
	if (findItr != m_GameObjects.end())
		m_GameObjects.erase(findItr);
}

void SceneFactory::AddSystem(const SharedPtr<powe::SystemBase>& system)
{
	if (std::ranges::find(m_Systems, system) == m_Systems.end())
		m_Systems.emplace_back(system);
}
