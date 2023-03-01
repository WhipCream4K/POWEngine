#include "pch.h"
#include "Application.h"

#include "POWEngine/Services/ServiceLocator.h"
#include "POWEngine/Logger/Console/ConsoleLogger.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Window/Window.h"
#include "POWEngine/Debug/imgui/ImGUI.h"
#include "POWEngine/Debug/imgui/ImGUISFMLDrawSystem.h"
#include "POWEngine/Debug/imgui/ImGUISFMLUpdateSystem.h"

uint8_t powe::Application::WindowIDCounter{};

powe::Application::Application() = default;

powe::Application::~Application() = default;

void powe::Application::Run()
{
    ServiceLocator::RegisterLogger(std::make_shared<powe::ConsoleLogger>());
    
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
    
    m_WorldEntity = std::make_unique<WorldEntity>();

    // need to put this before every draw system to be the last one updated
    // Draw Imgui Draw System
    m_WorldEntity->RegisterSystem(PipelineLayer::IssueRenderCommand,ImGUISFMLDrawSystem{});
    
    OnWorldInitialize(*m_WorldEntity);

#if USE_IMGUI & USE_SFML_WINDOW
    
    // Initialize ImGUI through game object
    GameObjectID imguiObject{m_WorldEntity->GetNewEntityID()};
    m_WorldEntity->RegisterGameObject(imguiObject);
    m_WorldEntity->AddComponentToGameObject(imguiObject,ImGUIComponent{});

    // Update Imgui System
    m_WorldEntity->RegisterSystem(PipelineLayer::InputValidation,ImGUISFMLUpdateSystem{});

#endif

    
    m_EngineCore.StartWorldClock();

    bool shouldQuit{};
    while (!shouldQuit)
    {
        shouldQuit = m_EngineCore.TranslateWindowInputs(*m_GameWindow,*m_WorldEntity);
        m_EngineCore.Step(*m_WorldEntity);
        m_EngineCore.Draw(m_GameWindowRenderer,*m_WorldEntity);
    }
    
}
