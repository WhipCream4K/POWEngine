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
}

void SceneFactory::AddGameObject(const SharedPtr<powe::GameObject>& gameObject)
{
	if (std::ranges::find(m_GameObjects, gameObject) == m_GameObjects.end())
		m_GameObjects.emplace_back(gameObject);
}

void SceneFactory::AddSystem(const SharedPtr<powe::SystemBase>& system)
{
	if (std::ranges::find(m_Systems, system) == m_Systems.end())
		m_Systems.emplace_back(system);
}
