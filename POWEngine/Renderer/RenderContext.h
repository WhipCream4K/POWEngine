#pragma once

#include <glm/glm.hpp>

namespace powe
{
    class Viewport;
    class Window;
    class Buffer;
    class Texture;
    class Shader;

    // Abstract class to comnbine different RHIs
    class RenderContext
    {
    public:

        RenderContext() = default;
        virtual ~RenderContext() = default;

        static SharedPtr<RenderContext> CreateDefault() noexcept;

        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
        virtual void SetClearColor(const glm::fvec4& color) noexcept = 0;
        virtual void SetViewport(const Viewport& viewport) noexcept = 0;
    
        // virtual SharedPtr<Buffer> CreateBuffer(size_t size, const void* data, bool isDynamic = false) = 0;
        // virtual void UpdateBuffer(const SharedPtr<Buffer>& buffer, const void* data, size_t size) = 0;
        // virtual void DeleteBuffer(const SharedPtr<Buffer>& buffer) = 0;
    
        // virtual SharedPtr<Texture> CreateTexture(int width, int height, int channels, const void* data) = 0;
        // virtual void UpdateTexture(const SharedPtr<Texture>& texture, const void* data) = 0;
        // virtual void DeleteTexture(const SharedPtr<Texture>& texture) = 0;
    
        // virtual SharedPtr<Shader> CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) = 0;
        // virtual void DeleteShader(const SharedPtr<Shader>& shader) = 0;
    
        virtual void SetVertexBuffer(const SharedPtr<Buffer>& buffer) = 0;
        virtual void SetIndexBuffer(const SharedPtr<Buffer>& buffer) = 0;

        // TODO: Implement
        // virtual void SetShader(Shader* shader) = 0;
    
        // virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1, 
        //                      uint32_t firstIndex = 0, int32_t vertexOffset = 0, 
        //                      uint32_t firstInstance = 0) = 0;
    };
}

