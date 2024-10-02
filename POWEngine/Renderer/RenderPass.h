#pragma once
#include <functional>

#include "Viewport.h"


namespace powe
{
    class RenderContext;
    class RenderPass
    {
        using RenderFunction = std::function<void(RenderContext&)>;
        
    public:
        
        RenderPass(std::string_view passName, RenderFunction func, Viewport::Flag requireFlag = Viewport::Flag::None);

        std::string_view GetName() const { return m_Name; }
        const Vector<std::string>& GetDependencies() const { return m_Dependencies; }
        void AddDependency(std::string_view passName);
        void Execute(RenderContext& ctx) const;

    private:

        std::string m_Name;

        // This needs dependencies pass to exist
        Vector<std::string> m_Dependencies;
        std::function<void(RenderContext&)> m_ExecuteFunction;
        Viewport::Flag m_RequireFlag;
    };
}
