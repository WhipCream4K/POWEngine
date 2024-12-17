#include "pch.h"

#include "OpenGLModule.h"
#include "OpenGLRenderContext.h"

using namespace powe;

void OpenGLModule::OnCreate(ModulesManager* modulesManager)
{
    auto appResource{ modulesManager->GetModuleResource<OpenGLModule>() };
    m_RenderContext = AllocateShared<OpenGLRenderContext>(appResource.get());
}