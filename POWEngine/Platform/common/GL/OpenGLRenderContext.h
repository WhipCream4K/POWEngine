#pragma once

#include "Renderer/RenderContext.h"
#include <glad/gl.h>

namespace powe
{

    class OpnenGLBuffer;
    class OpenGLRenderContext : public RenderContext
    {
    public:

        OpenGLRenderContext();
        virtual ~OpenGLRenderContext() override;

        void BeginFrame() override;
        void EndFrame() override {} // no need
        void SetViewport(const Viewport& viewport) noexcept override;
        void SetClearColor(const glm::fvec4& color) noexcept override;
        void SetVertexBuffer(const SharedPtr<Buffer>& buffer) override;
        void SetIndexBuffer(const SharedPtr<Buffer>& buffer) override;

        // TODO: Implement
        // void SetShader(Shader* shader) override;

        GladGLContext& GetContext() noexcept { return m_Context; }

    private:

        GladGLContext m_Context;
        glm::fvec4 m_ClearColor;
    };
}