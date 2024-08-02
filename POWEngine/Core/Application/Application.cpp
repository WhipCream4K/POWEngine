#include "pch.h"
#include "Application.h"
#include "Core/WindowManager.h"
#include "Core/Thread/SimpleThreadPool.h"
#include "Core/Logger/Console/ConsoleLogger.h"

powe::Application::Application(const AppDesc&)
	: m_LayerStack{ &m_TrackAllocator }
	, m_WindowManager{ AllocateUnique<WindowManager>(&m_TrackAllocator) }
	, m_ServiceLocator{ AllocateUnique<ServiceLocatorT>(ServiceLocatorT(&m_TrackAllocator),&m_TrackAllocator) }
{
	m_Instance = this;
}

void powe::Application::PopLayer()
{
	Layer* topLayer{ m_LayerStack.back().get() };
	topLayer->OnDetach();
	m_LayerStack.pop_back();
}

void powe::Application::Run()
{
	m_ServiceLocator->RegisterService(ConsoleLogger(&m_TrackAllocator));
	m_ServiceLocator->RegisterService(SimpleThreadPool(&m_TrackAllocator));
}
