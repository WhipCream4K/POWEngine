#include "pch.h"
#include "Application.h"
#include "POWEngine/Core/Core.h"
#include "POWEngine/Services/ServiceLocator.h"
#include "POWEngine/Logger/Console/ConsoleLogger.h"
#include "POWEngine/Sound/FMOD/FMOD2DSound.h"
#include "POWEngine/Window/Window.h"

uint8_t powe::Application::WindowIDCounter{};

void powe::Application::Run()
{
    ServiceLocator::RegisterLogger(std::make_shared<powe::ConsoleLogger>());
    
    m_EngineCore = std::make_shared<Core>();
    
    EngineProps engineProps{};
    engineProps.renderer = &m_EngineCore->GetRenderer();
    
    OnEngineSetUp(engineProps);

    const auto mainWindow = std::make_unique<Window>(
        engineProps.winProps.width,
        engineProps.winProps.height,
        engineProps.winProps.windowName,WindowIDCounter);

    // if(!engineProps.winProps.startWithVSync)
    //     mainWindow->SetFramerateLimit(144);
    
    mainWindow->SetVSync(engineProps.winProps.startWithVSync);
    mainWindow->SetClearColor(engineProps.winProps.clearColor);

    m_GameWindow = mainWindow.get();

    m_OpenWindows.try_emplace(mainWindow->GetID(),std::move(mainWindow));

    m_WorldEntity = std::make_unique<WorldEntity>(m_EngineCore);
    OnWorldInitialize(*m_WorldEntity);

    m_EngineCore->StartWorldClock();

    // updates game window
    while (!m_EngineCore->TranslateWindowInputs(*m_GameWindow,*m_WorldEntity))
    {
        // WaitForFrameN-2Display
        // Do Windows Messages
        // Update WorldInputSettings
        m_EngineCore->Step(*m_WorldEntity);
        m_EngineCore->Draw(*m_GameWindow,*m_WorldEntity);
    }
    
}
