#pragma once

#include <glm/glm.hpp>

namespace powe
{
    class Viewport;
    class Window;
    class Buffer;
    // Abstract class to separate the render engine from RHIs
    class RenderContext
    {
    public:

        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;
    
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
    
        virtual void Clear(const glm::fvec4&) = 0;
        virtual void SetViewport(const Viewport& viewport) = 0;
    
        virtual Buffer* CreateBuffer(size_t size, const void* data, bool isDynamic = false) = 0;
        virtual void UpdateBuffer(Buffer* buffer, const void* data, size_t size) = 0;
        virtual void DeleteBuffer(Buffer* buffer) = 0;
    
        virtual Texture* CreateTexture(int width, int height, int channels, const void* data) = 0;
        virtual void UpdateTexture(Texture* texture, const void* data) = 0;
        virtual void DeleteTexture(Texture* texture) = 0;
    
        virtual Shader* CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) = 0;
        virtual void DeleteShader(Shader* shader) = 0;
    
        virtual void SetVertexBuffer(Buffer* buffer) = 0;
        virtual void SetIndexBuffer(Buffer* buffer) = 0;

        // TODO: Implement
        // virtual void SetShader(Shader* shader) = 0;
    
        virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1, 
                             uint32_t firstIndex = 0, int32_t vertexOffset = 0, 
                             uint32_t firstInstance = 0) = 0;
    };
}

