#pragma once

#include "POWEngine/Core/CustomTypes.h"


namespace powe
{
	class SystemBase;
	class GameObject;
	class WorldEntity;
}

class SceneFactory
{
public:

	virtual void LoadScene(powe::WorldEntity&) = 0;
	virtual void UnloadScene(powe::WorldEntity&);

	virtual ~SceneFactory() = default;

protected:

	void AddGameObject(const SharedPtr<powe::GameObject>& gameObject);
	void AddSystem(const SharedPtr<powe::SystemBase>& system);

private:

	std::vector<SharedPtr<powe::GameObject>> m_GameObjects;
	std::vector<SharedPtr<powe::SystemBase>> m_Systems;

};

