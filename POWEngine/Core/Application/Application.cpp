#include "pch.h"
#include "Application.h"
#include "Core/WindowManager.h"
#include "Core/Logger/Console/ConsoleLogger.h"
#include "Core/Logger/LoggerUtils.h"

powe::Application::Application(const AppDesc& appDesc)
    : m_AppDesc(appDesc)
{
    m_Instance = this;

    auto* appResource{DefaultAllocator::Application};

    m_ServiceLocator = AllocateUnique<ServiceLocatorT>(appResource, appResource);
    m_LayerStack = Vector<UniquePtr<Layer>>{appResource};
    m_WindowManager = AllocateUnique<WindowManager>(appResource, appResource);

    m_ServiceLocator->RegisterService<SimpleThreadPool>(appResource);
    m_ServiceLocator->RegisterService<ConsoleLogger>(appResource);
}

powe::Application::~Application()
{
    DefaultAllocator::ShutDown();
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

        for (auto& layer : m_LayerStack)
        {
            layer->OnWindowEvents(windowEvents);
            layer->OnUpdate(m_Clock.GetDeltaTime());
        }

        m_Clock.End();
    }
}
