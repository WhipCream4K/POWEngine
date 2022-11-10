#include "pch.h"
#include "Application.h"
#include "POWEngine/Core/Core.h"
#include "POWEngine/Services/ServiceLocator.h"
#include "POWEngine/Logger/Console/ConsoleLogger.h"
#include "POWEngine/Sound/FMOD/FMOD2DSound.h"
#include "POWEngine/Window/Window.h"

void powe::Application::Run()
{
    ServiceLocator::RegisterLogger(std::make_shared<powe::ConsoleLogger>());
    
    m_EngineCore = std::make_unique<Core>();
    
    EngineProps engineProps{};
    engineProps.renderer = m_EngineCore->GetRenderer();
    
    OnEngineSetUp(engineProps);
    
    m_MainWindow = std::make_unique<Window>(
        engineProps.winProps.width,
        engineProps.winProps.height,
        engineProps.winProps.windowName);

    m_MainWindow->SetVSync(engineProps.winProps.startWithVSync);
    m_MainWindow->SetClearColor(engineProps.winProps.clearColor);

    m_WorldEntity = std::make_unique<WorldEntity>();
    OnWorldInitialize(*m_WorldEntity);

    m_EngineCore->StartWorldClock();
    
    while (!m_EngineCore->TranslateWindowInputs(m_MainWindow,m_WorldEntity))
    {
        m_EngineCore->Step(*m_WorldEntity);
        m_EngineCore->Draw(*m_MainWindow,*m_WorldEntity);
    }
    
}
