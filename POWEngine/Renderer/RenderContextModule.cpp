#include "pch.h"

#include "RenderContextModule.h"
#include "RenderContext.h"


using namespace powe;

RenderContextModule::RenderContextModule()
    : IModule("RenderContextModule")
{

}

void RenderContextModule::OnCreate(ModulesManager* modulesManager)
{
    auto resc{ modulesManager->GetModuleResource<RenderContextModule>() };
    m_RenderContextEvent = AllocateShared<RenderContextEvent>(resc,*this);

    Application::Get().RegisterAppEvent(m_RenderContextEvent);
}

void RenderContextEvent::OnSetup()
{
    m_RefModule->SetMainRenderContext(RenderContext::CreateDefault());
}