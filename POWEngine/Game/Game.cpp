#include "pch.h"
#include "Game.h"

#include "GameEvent.h"
#include "Scene.h"
#include "Core/Application/Application.h"
#include "Core/ModulesManager.h"
#include "Core/Memory/AllocatorContext.h"
#include "Game/Input/InputManager.h"
#include "SceneViewport.h"

using namespace powe;

Game::Game()
    : IModule("Game")
    , m_InputManager(*this)
{
}

Game::~Game() = default;

void Game::OnCreate(ModulesManager* modulesManager)
{
    const auto resource{ modulesManager->GetModuleResource<Game>() };

    m_SceneMap = UnOrderedMap<std::string, UniquePtr<Scene>>{ resource };

    m_GameEvent = AllocateShared<GameEvent>(resource);

    auto& app{Application::Get()};
    app.RegisterAppEvent(m_GameEvent);

    // Set default bind window to the main window
    SetBindWindow(app.GetAppWindow());

}

Scene* Game::CreateScene(std::string_view sceneName) noexcept
{
    if (m_SceneMap.find(sceneName.data()) != m_SceneMap.end())
    {
        return m_SceneMap[sceneName.data()].get();
    }

    const AllocatorContext context{AllocatorScope::Game};
    auto* upStream{context.GetResource()};

    UniquePtr<Scene> scene{ AllocateUnique<Scene>(upStream, *this) };
    
    if(m_GameEvent->GetActiveScene() == nullptr)
    {
        m_GameEvent->SetActiveScene(scene.get());
    }

    m_SceneMap[std::string(sceneName)] = std::move(scene);

    return m_SceneMap[sceneName.data()].get();
}

void Game::RemoveScene(std::string_view sceneName) noexcept
{
    if(IsInAppMainThread())
    {
        if (m_SceneMap.find(sceneName.data()) != m_SceneMap.end())
        {
            m_SceneMap.erase(sceneName.data());
        }
    }

}

std::string_view Game::GetSceneName(const Scene* scene) const noexcept
{
    return std::ranges::find_if(m_SceneMap, [scene](const auto& pair)
    {
        return pair.second.get() == scene;
    })->first;
}

void Game::SetActiveScene(Scene* scene) noexcept
{
    m_GameEvent->SetActiveScene(scene);
}

Scene* Game::GetActiveScene() const noexcept
{
    return m_GameEvent->GetActiveScene();
}

void Game::SetBindWindow(Window* window) noexcept
{
    m_BindWindow = window;
    auto windowViewport{ window->GetViewport() };

    auto allocator{Application::Get().GetModulesManager().GetModuleResource<Game>() };
    
    // default game viewport
    auto sceneViewport = AllocateShared<SceneViewport>(allocator, 
    windowViewport->GetPosition(), windowViewport->GetSize());

    sceneViewport->BindWindow(window);

    m_SceneViewport = std::move(sceneViewport);
}

void Game::OnExit(ModulesManager*)
{
    m_SceneMap.clear();
}
