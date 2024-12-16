#include "pch.h"
#include "SceneRenderEvent.h"
#include "RenderGraph.h"
#include "Game/Game.h"
#include "SceneRenderer.h"

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
}

void SceneRenderEvent::OnUpdate(float)
{   
    SceneRenderGraph& renderGraph{m_SceneRenderer->GetSceneRenderGraph()};
    renderGraph.SetDrawScene(m_GameModule->GetActiveScene());

    m_SceneRenderer->Draw();
}