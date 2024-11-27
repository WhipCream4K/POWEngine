#include "pch.h"
#include "Application.h"
#include "Core/Window/WindowManager.h"
#include "Core/Memory/MemoryManager.h"
#include "Core/Memory/Allocator.h"
#include "Logger/Logger.h"
#include "Utils/Utils.h"
#include "Core/ModulesManager.h"
#include "Core/Application/AppEvent.h"

powe::Application::Application(const AppDesc& appDesc)
    : m_AppDesc(appDesc)
{
    m_Instance = this;

    // Initilaize Application memory resource
    SharedPtr<TrackableAllocator> appResource{std::make_shared<TrackableAllocator>()};
    m_AppAllocator = appResource.get();

    MemoryManager::Init(appResource);

    // Init WindowManager
    m_WindowManager = AllocateUnique<WindowManager>(m_AppAllocator,appResource);

    // Init Modules
    m_AppModules = AllocateUnique<ModulesManager>( m_AppAllocator);

    const auto loggerResource{MemoryManager::Get()->NewAllocator("Logger")};
    m_AppModules->CreateModule<Logger>(loggerResource);

    // Assign Thread ID
    m_AppThreadID = std::this_thread::get_id();

}

powe::Application::~Application()
{
}

void powe::Application::Run()
{
    powe::Info("Application is running");

    m_Clock.ResetTime();

    if(m_WindowManager)
    {
        m_AppWindow = m_WindowManager->CreateWindow(m_AppDesc.Name, m_AppDesc.Width, m_AppDesc.Height);
    }

    while (!m_AppWindow->IsClosed())
    {
        m_Clock.Start();

        m_WindowManager->Update();
        
        for(auto& appEvent : m_AppEvents)
        {
            appEvent->OnUpdate(m_Clock.GetDeltaTime());
        }

        m_Clock.End();
    }
}

void powe::Application::RemoveAppEvent(const SharedPtr<AppEvent>& appEvent)
{
    if(!IsInMainThread())
    {
        powe::Warning("RemoveAppEvent can only be called from main thread");
        return;
    }

    auto findItr = std::ranges::find_if(m_AppEvents, [appEvent](const SharedPtr<AppEvent>& event)
    {
        return event == appEvent;
    });

    if (findItr != m_AppEvents.end())
    {
        m_AppEvents.erase(findItr);
    }
}
