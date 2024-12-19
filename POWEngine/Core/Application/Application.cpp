#include "pch.h"

#include "Application.h"
#include "Core/Window/WindowManager.h"
#include "Core/Memory/MemoryManager.h"
#include "Core/Memory/Allocator.h"
#include "Logger/Logger.h"
#include "Utils/Utils.h"
#include "Core/ModulesManager.h"
#include "Core/Application/AppEvent.h"
#include "Core/Thread/SimpleThreadPool.h"
#include "Game/Game.h"
#include "Game/GameEvent.h"
#include "Renderer/SceneRenderEvent.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/RenderContextModule.h"

#include "Platform/common/GLFW/glfwModule.h"

powe::Application* powe::Application::m_Instance = nullptr;

powe::Application::Application(const AppDesc& appDesc)
    : m_AppDesc(appDesc)
{
    m_Instance = this;

    // Initilaize Application memory resource
    SharedPtr<TrackableAllocator> appResource{std::make_shared<TrackableAllocator>()};
    m_AppAllocator = appResource.get();

    MemoryManager::Init(appResource);

    MemoryManager::Get()->RegisterAllocator("Application", appResource);

    // Init WindowManager
    m_WindowManager = AllocateUnique<WindowManager>(appResource);

    // Init Modules
    m_AppModules = AllocateUnique<ModulesManager>( m_AppAllocator);

    // Logger
    const auto loggerResource{MemoryManager::Get()->NewAllocator("Logger")};
    m_AppModules->CreateModule<Logger>(loggerResource);

    // SimpleThreadPool
    const auto simeleThreadPoolResource{MemoryManager::Get()->NewAllocator("ThreadPool")};
    m_AppModules->CreateModule<SimpleThreadPool>(simeleThreadPoolResource);

    // GLFW module
    m_AppModules->CreateModule<glfwModule>();

    // Game module
    const auto gameResource{MemoryManager::Get()->NewAllocator("Game")};
    m_AppModules->CreateModule<Game>(gameResource);

    // Render context module
    m_AppModules->CreateModule<RenderContextModule>(gameResource); 

    // Scene Rendering module
    m_AppModules->CreateModule<SceneRenderer>(gameResource);

    // Default AppEvent setup
    m_AppEventSetupLogic = [](powe::AppEventSetup& appEventSetup){
        appEventSetup.Add<glfwEvent>();
        appEventSetup.Add<GameEvent>();
        appEventSetup.Add<RenderContextEvent>();
        appEventSetup.Add<SceneRenderEvent>();
    };

    // Assign Thread ID
    m_AppThreadID = std::this_thread::get_id();

}

powe::Application::~Application() = default;

void powe::Application::Run()
{
    powe::Info("Application is running");

    if(m_WindowManager)
    {
        m_AppWindow = m_WindowManager->CreateWindow(m_AppDesc.name, m_AppDesc.width, m_AppDesc.height);
    }
    
    // Initialize AppEvent
    powe::AppEventSetup m_AppEventSetup{m_AppEvents};
    m_AppEventSetupLogic(m_AppEventSetup);
    m_AppEventSetup.Sort();

    for(auto& appEvent : m_AppEvents)
    {
        appEvent->OnSetup();
    }

    m_Clock.ResetTime();

    while (!m_AppWindow->IsClosed())
    {
        m_Clock.Start();
        
        for(auto& appEvent : m_AppEvents)
        {
            appEvent->OnUpdate(m_Clock.GetDeltaTime());
        }

        m_Clock.End();
    }

    m_AppModules->Clear();
}

void powe::Application::RegisterAppEvent(const SharedPtr<AppEvent>& appEvent) noexcept
{
    m_AppEvents.emplace_back(appEvent);
}