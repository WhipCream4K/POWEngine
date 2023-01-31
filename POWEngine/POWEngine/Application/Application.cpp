#include "pch.h"
#include "Application.h"
// #include "POWEngine/Core/Core.h"
#include "POWEngine/Services/ServiceLocator.h"
#include "POWEngine/Logger/Console/ConsoleLogger.h"
#include "POWEngine/Sound/FMOD/FMOD2DSound.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Window/Window.h"
#include "POWEngine/Renderer/RenderAPI.h"
#include <unordered_map>

#include "POWEngine/Core/Clock/WorldClock.h"
#include "POWEngine/Core/Input/InputManager.h"

uint8_t powe::Application::WindowIDCounter{};

powe::Application::Application() = default;

powe::Application::~Application() = default;

void powe::Application::Run()
{
    ServiceLocator::RegisterLogger(std::make_shared<powe::ConsoleLogger>());
    ServiceLocator::RegisterAppInterface(shared_from_this());
    
    // m_EngineCore = std::make_shared<Core>();
    
    // ServiceLocator::RegisterCoreInterface(m_EngineCore);
    
    EngineProps engineProps{};
    
    OnEngineSetUp(engineProps);
    
    const auto mainWindow = std::make_shared<Window>(
        engineProps.winProps.width,
        engineProps.winProps.height,
        engineProps.winProps.windowName,WindowIDCounter);

    // if(!engineProps.winProps.startWithVSync)
    //     mainWindow->SetFramerateLimit(144);
    
    mainWindow->SetVSync(engineProps.winProps.startWithVSync);
    mainWindow->SetClearColor(engineProps.winProps.clearColor);

    m_GameWindowRenderer.SetRenderAPI(std::move(engineProps.mainWindowRenderAPI));
    m_GameWindowRenderer.SetTargetWindow(mainWindow);
    
    m_GameWindow = mainWindow;
    // const uint8_t windowID{mainWindow->GetID()};

    // m_OpenWindows.try_emplace(windowID,std::move(mainWindow));

    m_WorldEntity = std::make_unique<WorldEntity>();
    OnWorldInitialize(*m_WorldEntity);

    m_EngineCore.StartWorldClock();

    bool shouldQuit{};
    while (!shouldQuit)
    {
        shouldQuit = m_EngineCore.FullStep(m_GameWindowRenderer,*m_WorldEntity);
        // shouldQuit = m_EngineCore.TranslateWindowInputs(*m_GameWindow,*m_WorldEntity);
        // m_EngineCore.Step(m_GameWindowRenderer,*m_WorldEntity);
        
        // m_EngineCore->Step(*m_GameWindow,*m_WorldEntity);
        
        // m_EngineCore->Step(*m_WorldEntity);
        // m_EngineCore->DisplayBackbuffer(*m_GameWindow);
        // m_EngineCore->Draw(*m_GameWindow,*m_WorldEntity);
    }
    
}
