#pragma once

#include "Core/IModule.h"

namespace powe 
{
    class OpenGLRenderContext;
    class OpenGLModule : public IModule
    {
    public:
    
        virtual void OnCreate(ModulesManager*) override;
        virtual void OnExit(ModulesManager*) override;

    private:

        SharedPtr<OpenGLRenderContext> m_RenderContext;
    };
}