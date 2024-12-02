#include "pch.h"

#include <execution>

#include "Scene.h"
#include "Game.h"
#include "SceneSystem.h"


#include "Core/Thread/SimpleThreadPool.h"

using namespace powe;

Scene::Scene(Game& game)
	: m_Game(game)
{
}

void Scene::Start()
{
}

void Scene::Exit()
{
}

void Scene::Update(float deltaTime)
{
}

SharedPtr<PMRResource> Scene::GetResource() const noexcept
{
	return m_Game->GetResource();
}

std::string_view Scene::GetName() const noexcept
{
	return m_Game->GetSceneName(this);
}
