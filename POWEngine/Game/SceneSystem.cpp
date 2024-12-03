#include "pch.h"
#include "SceneSystem.h"

#include "Scene.h"

using namespace powe;

SceneSystem::SceneSystem(Scene &sceneRef) : m_Scene(sceneRef) {}

ECSManager& SceneSystem::GetECSManager() const noexcept 
{
    return m_Scene->GetECSManager();
}

