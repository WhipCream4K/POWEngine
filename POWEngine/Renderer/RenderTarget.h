#pragma once


namespace powe
{
    class RenderContext;
    class RenderTarget
    {
    public:

        virtual void Present(RenderContext& context);
        virtual void Clear(RenderContext& context);
        virtual ~RenderTarget() = default;
    };
}

