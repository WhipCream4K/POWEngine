#pragma once

#include "Renderer/RenderContext.h"
#include <glad/gl.h>

namespace powe
{
    class OpnenGLBuffer;
    class OpenGLRenderContext : public RenderContext
    {
    public:

        OpenGLRenderContext(uint32_t renderBufferCount);
        virtual ~OpenGLRenderContext() override;

        void BeginFrame() override;
        void EndFrame() override;
        void Clear(const glm::fvec4&) override;
        void SetViewport(const Viewport& viewport) override;
        
        void SetVertexBuffer(const SharedPtr<Buffer>& buffer) override;
        void SetIndexBuffer(const SharedPtr<Buffer>& buffer) override;

        // TODO: Implement
        // void SetShader(Shader* shader) override;

        GladGLContext& GetContext() noexcept { return m_Context; }

    private:

        GladGLContext m_Context;
        Vector<GLuint> m_RenderBuffers;
        uint32_t m_ActiveRenderBuffer;
    };
}