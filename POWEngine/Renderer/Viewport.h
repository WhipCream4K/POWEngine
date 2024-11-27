#pragma once

namespace powe
{
    class RenderTarget;
    class RenderContext;
    class Viewport
    {
    public:

        Viewport(PMRResource* memResource);
        virtual ~Viewport() = default;

        virtual void Init(RenderContext& ctx) = 0;
        virtual void* GetSubSystem() = 0;
    };
}

