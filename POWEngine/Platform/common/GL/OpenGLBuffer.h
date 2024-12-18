#pragma once

#include "Renderer/Buffer.h"
#include <glad/gl.h>

namespace powe 
{
    class OpenGLBuffer : public Buffer
    {
    public:
    
        OpenGLBuffer(GladGLContext* context,size_t size, Type type, Usage usage);
        virtual ~OpenGLBuffer();

        void* Map() noexcept override {return nullptr;}
        void Unmap() noexcept override {}
        void Update(const void* data, size_t size, size_t offset = 0) noexcept override;

    private:

        GladGLContext* m_Context;
        GLuint m_Handle;
        GLenum m_GLType;
    };
}   