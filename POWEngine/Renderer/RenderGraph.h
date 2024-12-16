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

        SceneRenderGraph() = default;
        
        void Execute(RenderContext& ctx);
        void AddPass(UniquePtr<RenderPass>&& pass);
        void SetDrawScene(Scene* scene) noexcept { m_DrawScene = scene; }
        const Scene* GetDrawScene() const noexcept { return m_DrawScene; }

    private:

        void ResolvePass();

        Vector<UniquePtr<RenderPass>> m_RenderPasses;
        Scene* m_DrawScene;
    };
}
