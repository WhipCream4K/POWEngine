#include "pch.h"
#include "Game.h"

#include "GameEvent.h"
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
    m_Scenes = Vector<SharedPtr<Scene>>{ resource.get() };
    
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

void Game::OnExit(ModulesManager* modulesManager)
{
    m_Scenes.clear();
}
