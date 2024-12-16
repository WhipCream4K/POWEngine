#pragma once

#include "Renderer/Buffer.h"
#include <glad/gl.h>

namespace powe
{
    class glfwBuffer : public Buffer
    {
    public:

        glfwBuffer(size_t size, BufferType type) : Buffer(size, type) {};
        virtual ~glfwBuffer() override = default;

        virtual void* Map() noexcept override = 0;
        virtual void Unmap() noexcept override = 0;
        virtual void Update(const void* data, size_t size, size_t offset = 0) noexcept override = 0;

    
    };

}