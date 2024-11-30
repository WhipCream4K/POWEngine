#include "pch.h"

#include "Scene.h"
#include "Game.h"
#include "SceneEvent.h"

using namespace powe;

Scene::Scene(Game& game)
	: m_Game(game)
	, m_InputManager(*this)
{
}

void Scene::Start()
{
	m_SceneEvent->OnStart(*this);
}

void Scene::Exit()
{
	m_SceneEvent->OnExit(*this);
}

void Scene::Update(float deltaTime)
{
	m_SceneEvent->OnUpdate(*this);
}

void Scene::CallCreateSceneEvent(SceneEvent* sceneEvent)
{
	sceneEvent->OnCreate(*this);
}

SharedPtr<PMRResource> Scene::GetResource() const noexcept
{
	return m_Game->GetResource();
}

std::string_view Scene::GetName() const noexcept
{
	return m_Game->GetSceneName(this);
}
