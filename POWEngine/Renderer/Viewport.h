#pragma once

namespace powe
{
    class RenderTarget;
    class RenderContext;
    class Viewport
    {
    public:

        void SetRenderTarget(const SharedPtr<RenderTarget>& renderTarget) {m_RenderTarget = renderTarget;}
        RenderTarget* GetRenderTarget() const { return m_RenderTarget.get(); }
        void Present(RenderContext& context) const;
        void Clear(RenderContext& ctx) const;        

    private:

        SharedPtr<RenderTarget> m_RenderTarget;
    };
}

