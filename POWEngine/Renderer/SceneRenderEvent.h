#pragma once

#include "Core/Application/AppEvent.h"

namespace powe
{
    class Game;
    class SceneRenderer;
    class SceneRenderEvent : public AppEvent
    {
    public:
    
        SceneRenderEvent(SceneRenderer& sceneRenderer) noexcept;
        virtual ~SceneRenderEvent() = default;

        void OnSetup() override;
        void OnUpdate(float deltaTime) override;

    private:

        // Game module would likely outlive SceneRenderEvent
        // Since SceneRenderEvent is owned by SceneRenderer
        // and have to recreate if SceneRenderer is recreated
        Game* m_GameModule;
        
        RefWrap<SceneRenderer> m_SceneRenderer;
    };
}