#include "pch.h"
#include "OpenGLBuffer.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

using namespace powe;

constexpr GLenum ConvertToGLUsage(Buffer::Usage usage)
{
    // Base usage
    GLenum glUsage = GL_STREAM_DRAW;  // Default to GL_STREAM_DRAW

    // Determine frequency of access
    if (usage & int(Buffer::Usage::Static))
        glUsage = GL_STATIC_DRAW;
    else if (usage & int(Buffer::Usage::Dynamic))
        glUsage = GL_DYNAMIC_DRAW;
    // else it remains GL_STREAM_DRAW

    // Determine nature of access
    if (usage & int(Buffer::Usage::Draw))
        ; // Already set to *_DRAW
    else if (usage & int(Buffer::Usage::Read))
        glUsage = static_cast<GLenum>(glUsage + 1);  // *_READ is always one more than *_DRAW
    else if (usage & int(Buffer::Usage::Copy))
        glUsage = static_cast<GLenum>(glUsage + 2);  // *_COPY is always two more than *_DRAW

    return glUsage;
}

OpenGLBuffer::OpenGLBuffer(GladGLContext* context,size_t size, Type type, Usage usage)
    : Buffer(size, type, usage)
    , m_Context(context)
{
    switch (type)
    {
        case Type::Vertex: m_GLType = GL_ARRAY_BUFFER; break;
        case Type::Index: m_GLType = GL_ELEMENT_ARRAY_BUFFER; break;
        case Type::Uniform: m_GLType = GL_UNIFORM_BUFFER; break;
        case Type::Storage: m_GLType = GL_SHADER_STORAGE_BUFFER; break;
    }

    m_Context->GenBuffers(1, &m_Handle);
    m_Context->BindBuffer(m_GLType, m_Handle);
    m_Context->BufferData(m_GLType, size, nullptr, ConvertToGLUsage(usage));
}

void OpenGLBuffer::Update(const void* data, size_t size, size_t offset) noexcept
{
    m_Context->BufferSubData(m_GLType, offset, size, data);
}