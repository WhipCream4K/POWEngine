#include "SceneFactory.h"

void SceneFactory::AddGameObject(const SharedPtr<powe::GameObject>& gb)
{
    if(std::ranges::find(m_GameObjects,gb) == m_GameObjects.end())
    {
        m_GameObjects.emplace_back(gb);
    }
}
