#include "pch.h"
#include "EngineLayer.h"

#include "Utils/Utils.h"
#include "Scene.h"

powe::EngineLayer::EngineLayer()
	: m_Scenes(&m_TrackAllocator)
	, m_ActiveScene(nullptr)
{
}

powe::Scene& powe::EngineLayer::CreatScene(std::string_view sceneName)
{
	auto scene = AllocateUnique<Scene>(&m_TrackAllocator,*this);
	m_Scenes[std::string(sceneName)] = std::move(scene);
	return *m_Scenes[sceneName.data()];
}

void powe::EngineLayer::OnAttach()
{
}

void powe::EngineLayer::OnDetach()
{

}

void powe::EngineLayer::OnUpdate(float deltaTime)
{
	m_ActiveScene->Update(deltaTime);
}

void powe::EngineLayer::OnWindowEvents(const Window::EventQueue&)
{
}
