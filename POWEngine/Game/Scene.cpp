#include "pch.h"

#include <execution>

#include "Scene.h"
#include "Game.h"
#include "SceneSystem.h"

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
	std::for_each(std::execution::par_unseq, m_UnsequenceSystems.begin(), m_UnsequenceSystems.end(), [&](SharedPtr<SceneSystem> system)
	{
		system->OnUpdate(*this, deltaTime);
	})
}

SharedPtr<PMRResource> Scene::GetResource() const noexcept
{
	return m_Game->GetResource();
}

std::string_view Scene::GetName() const noexcept
{
	return m_Game->GetSceneName(this);
}
