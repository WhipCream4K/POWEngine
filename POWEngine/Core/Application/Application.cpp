#include "pch.h"
#include "Application.h"
#include "Core/WindowManager.h"
#include "Logger/Console/ConsoleLogger.h"
#include "Core/Memory/MemoryManager.h"
#include "Core/Memory/Allocator.h"
#include "Logger/LoggerUtils.h"

powe::Application::Application(const AppDesc& appDesc)
    : m_AppDesc(appDesc)
{
    m_Instance = this;

    // Initilaize Application memory resource
    SharedPtr<TrackableAllocator> appResource{std::make_shared<TrackableAllocator>()};
    m_AppAllocator = appResource.get();

    m_ServiceLocator = AllocateUnique<ServiceLocatorT>(m_AppAllocator, appResource);
    m_LayerStack = Vector<UniquePtr<Layer>>{m_AppAllocator};
    m_WindowManager = AllocateUnique<WindowManager>(m_AppAllocator, m_AppAllocator);

    // Register Application memory resource
    MemoryManager* memManager{ m_ServiceLocator->RegisterService<MemoryManager>() };
    memManager->RegisterAllocator("Application", m_AppAllocator);    
    
    m_ServiceLocator->RegisterService<SimpleThreadPool>();
    m_ServiceLocator->RegisterService<ConsoleLogger>();
}

powe::Application::~Application()
{
}

void powe::Application::PopLayer()
{
    Layer* topLayer{m_LayerStack.back().get()};
    topLayer->OnDetach();
    m_LayerStack.pop_back();
}

void powe::Application::Run()
{
    POWE_LOG("Application is running");

    m_Clock.ResetTime();

    while (!m_WindowManager->GetMainWindow().ShouldClose())
    {
        m_Clock.Start();

        auto windowEvents = m_WindowManager->Update();

        // TODO: Change to module dependency based update
        
        for (auto& layer : m_LayerStack)
        {
            layer->OnWindowEvents(windowEvents);
            layer->OnUpdate(m_Clock.GetDeltaTime());
        }

        m_Clock.End();
    }
}
