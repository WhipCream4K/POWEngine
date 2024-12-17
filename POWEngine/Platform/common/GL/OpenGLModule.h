#pragma once

#include "Core/IModule.h"

namespace powe 
{
    class OpenGLRenderContext;
    class OpenGLModule : public IModule
    {
    public:
    
        OpenGLModule();

        virtual void OnCreate(ModulesManager*) override;
        virtual void OnExit(ModulesManager*) override;

        const SharedPtr<OpenGLRenderContext>& GetRenderContext() const noexcept { return m_RenderContext; }

    private:

        SharedPtr<OpenGLRenderContext> m_RenderContext;
    };
}