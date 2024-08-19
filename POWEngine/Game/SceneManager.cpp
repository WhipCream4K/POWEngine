#include "pch.h"
#include "SceneManager.h"

#include "Utils/Utils.h"
#include "Scene.h"

powe::SceneManager::SceneManager(PMRResource* memResource)
    : m_Scenes(memResource)
{
}

powe::Scene& powe::SceneManager::CreatScene(std::string_view sceneName) noexcept
{
    UniquePtr<Scene> scene = AllocateUnique<Scene>(DefaultAllocator::Engine);
    m_Scenes[std::string(sceneName)] = std::move(scene);
    return *m_Scenes[sceneName.data()];
}

void powe::SceneManager::RemoveScene(std::string_view sceneName) noexcept
{
    if (m_Scenes.find(sceneName.data()) != m_Scenes.end())
    {
        m_Scenes.erase(sceneName.data());
    }
}

powe::Scene* powe::SceneManager::GetScene(std::string_view sceneName) noexcept
{
    if (const auto it = m_Scenes.find(sceneName.data()); it != m_Scenes.end())
    {
        return it->second.get();
    }

    return nullptr;
}
