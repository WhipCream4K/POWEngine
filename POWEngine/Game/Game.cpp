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
    m_Scenes = Vector<UniquePtr<Scene>>{ resource.get() };
    
    m_GameEvent = std::allocate_shared<GameEvent>(resource);

    Application::Get().RegisterAppEvent(m_GameEvent);
}

void Game::OnExit(ModulesManager* modulesManager)
{
    m_Scenes.clear();
}
