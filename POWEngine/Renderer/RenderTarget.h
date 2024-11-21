#pragma once


namespace powe
{
    class RenderContext;
    class RenderTarget
    {
    public:

        enum Flag
        {
            None,
            Color,
            Depth
        };

        virtual void Present(RenderContext& renderContext,RenderTarget& other);
        virtual void Clear(RenderContext& context);
        virtual ~RenderTarget() = default;
    };
}

