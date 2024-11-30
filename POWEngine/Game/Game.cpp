#include "pch.h"
#include "Game.h"

#include "GameEvent.h"
#include "Scene.h"
#include "Core/Application/Application.h"
#include "Core/ModulesManager.h"

using namespace powe;

Game::Game()
    : IModule("Game")
{
}

void Game::OnCreate(ModulesManager* modulesManager)
{
    const auto resource{ modulesManager->GetModuleResource<Game>() };
    // m_Scenes = Vector<SharedPtr<Scene>>{ resource.get() };
    m_SceneMap = UnOrderedMap<std::string, UniquePtr<Scene>>{ resource.get() };
    
    m_GameEvent = std::allocate_shared<GameEvent>(resource);

    auto& app{Application::Get()};
    app.RegisterAppEvent(m_GameEvent);

    // Set default bind window to the main window
    m_BindWindow = app.GetAppWindow();
}

SharedPtr<PMRResource> Game::GetResource() const noexcept
{
    return Application::Get().GetModulesManager().GetModuleResource<Game>();
}

Scene* Game::CreateScene(std::string_view sceneName) noexcept
{
    if (m_SceneMap.find(sceneName.data()) != m_SceneMap.end())
    {
        return m_SceneMap[sceneName.data()].get();
    }

    UniquePtr<Scene> scene{ AllocateUnique<Scene>(GetResource(), *this) };
    
    if(m_ActiveScene == nullptr)
        m_ActiveScene = scene.get();

    m_SceneMap[std::string(sceneName)] = std::move(scene);

    return m_SceneMap[sceneName.data()].get();
}

std::string_view Game::GetSceneName(const Scene* scene) const noexcept
{
    return std::ranges::find_if(m_SceneMap, [scene](const auto& pair)
    {
        return pair.second.get() == scene;
    })->first;
}

void Game::OnExit(ModulesManager* modulesManager)
{
    m_SceneMap.clear();
}
