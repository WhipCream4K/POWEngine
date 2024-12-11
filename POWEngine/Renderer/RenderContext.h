#pragma once

namespace powe
{
    class Viewport;
    class Window;

    // Abstract class to separate the render engine from RHIs
    class RenderContext
    {
    public:

        // TODO: Add a way to draw geometries on viewport
        virtual void DrawGeometries() = 0;
        virtual void SetViewport(const Viewport&) = 0;
        virtual void ClearDrawBuffers() = 0;
    };
}

