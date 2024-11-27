#pragma once
#include <functional>

#include "Viewport.h"
// #include "RenderTarget.h"


namespace powe
{
    class RenderContext;
    class SceneRenderGraph;
    class RenderPass
    {        
    public:
        
        RenderPass(std::string_view passName);
        virtual ~RenderPass() = default;

        std::string_view GetName() const { return m_Name; }
        const Vector<std::string>& GetDependencies() const { return m_Dependencies; }
        void AddDependency(std::string_view passName);

        virtual void Execute(RenderContext& ctx,SceneRenderGraph& graph) const = 0;

    private:

        std::string m_Name;

        // This needs dependencies pass to exist
        Vector<std::string> m_Dependencies;
    };
}
