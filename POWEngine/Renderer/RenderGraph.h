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

        SharedPtr<Scene> GetScene() const noexcept { return m_Scene.lock(); }
        void SetScene(const SharedPtr<Scene>& scene) { m_Scene = scene; }

    private:

        void ResolvePass();

        Vector<UniquePtr<RenderPass>> m_RenderPasses;
        WeakPtr<Scene> m_Scene;
    };
}
