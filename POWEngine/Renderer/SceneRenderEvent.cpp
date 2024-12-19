#include "pch.h"
#include "SceneRenderEvent.h"
#include "RenderGraph.h"
#include "Game/Game.h"
#include "SceneRenderer.h"
#include "RenderContextModule.h"

using namespace powe;

SceneRenderEvent::SceneRenderEvent(SceneRenderer& sceneRenderer) noexcept
    : m_SceneRenderer(sceneRenderer)
{
}

void SceneRenderEvent::OnSetup()
{
    auto gameModuel{ Application::GetModule<Game>() };
    if(!gameModuel)
    {
        powe::Error("Game module not found, cannot setup scene render event");
        return;
    }

    m_GameModule = gameModuel.get();


    // Get the main rendering module
    auto renderContextModule{ Application::GetModule<RenderContextModule>() };
    if(renderContextModule)
    {
        m_SceneRenderer->SetRenderContext(renderContextModule->GetMainRenderContext());
    }
    else
    {
        powe::Error("RenderContextModule not found, cannot setup SceneRenderer");
    }
    
}

void SceneRenderEvent::OnUpdate(float)
{   
    SceneRenderGraph& renderGraph{m_SceneRenderer->GetSceneRenderGraph()};
    renderGraph.SetDrawScene(m_GameModule->GetActiveScene());
    renderGraph.SetViewport(m_GameModule->GetSceneViewport().get());

    m_SceneRenderer->Draw();
}