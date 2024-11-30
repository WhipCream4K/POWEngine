#include "pch.h"

#include "GameEvent.h"
#include "Scene.h"

using namespace powe;

GameEvent::GameEvent(Game &game)
    : m_Game(game)
{
}

void GameEvent::OnUpdate(float deltaTime)
{
    m_ActiveScene->Update(deltaTime);
}