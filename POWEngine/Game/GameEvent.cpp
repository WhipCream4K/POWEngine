#include "pch.h"

#include "GameEvent.h"
#include "Scene.h"

using namespace powe;

void GameEvent::OnUpdate(float deltaTime)
{
    m_ActiveScene->Update(deltaTime);
}