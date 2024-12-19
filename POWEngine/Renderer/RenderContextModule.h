#pragma once

#include "Core/IModule.h"
#include "Core/Application/AppEvent.h"

namespace powe
{
    class RenderContextModule;
    class RenderContextEvent : public AppEvent
    {
    public:
        
        constexpr RenderContextEvent(RenderContextModule& refModule)
            : m_RefModule{refModule}
        {
        }

        virtual ~RenderContextEvent() = default;

        void OnSetup() override;
        void OnUpdate(float) override {}

    private:

        RefWrap<RenderContextModule> m_RefModule;
    };

    class RenderContext;
    class RenderContextModule : public IModule
    {
    public:
        RenderContextModule();
        ~RenderContextModule() = default;

        void OnCreate(ModulesManager*) override;
        void OnExit(ModulesManager*) override {}
        void SetMainRenderContext(const SharedPtr<RenderContext>& context) noexcept { m_MainRenderContext = context; }
        RenderContext* GetMainRenderContext() const noexcept { return m_MainRenderContext.get(); }

    private:

        SharedPtr<RenderContext> m_MainRenderContext;
        SharedPtr<RenderContextEvent> m_RenderContextEvent;
    };
}