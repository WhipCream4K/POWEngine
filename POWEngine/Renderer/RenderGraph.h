#pragma once

namespace powe
{
    class RenderPass;
    class Scene;
    class RenderContext;
    class Viewport;
    class SceneRenderer;
    class SceneRenderGraph
    {
    public:

        SceneRenderGraph(uint32_t renderBufferCount = 2);

        void Execute(RenderContext& ctx);

        void AddPass(UniquePtr<RenderPass>&& pass);

        WeakPtr<Scene> Scene;
        WeakPtr<Viewport> Viewport;

    private:

        void ResolvePass();

        Vector<UniquePtr<RenderPass>> m_RenderPasses;
    };
}
