#include "SceneFactory.h"

void SceneFactory::AddGameObject(const SharedPtr<powe::GameObject>& gb)
{
    if(std::ranges::find(m_GameObjects,gb) == m_GameObjects.end())
    {
        m_GameObjects.emplace_back(gb);
    }
}

void SceneFactory::AddSystem(const SharedPtr<powe::SystemBase>& system)
{
    if(std::ranges::find(m_Systems,system) == m_Systems.end())
    {
        m_Systems.emplace_back(system);
    }
}
