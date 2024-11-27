#pragma once

#include "glm/glm.hpp"

namespace powe
{
    // Enum for PixelFormat
    enum class PixelFormat
    {
        RGBA8,
        RGB10A2,
        FLOAT16,
        FLOAT32
    };

    // Enum for DepthFormat
    enum class DepthFormat
    {
        Depth24,
        Depth32,
        Depth24Stencil8
    };

    // Enum for RenderTargetUsage
    enum class RenderTargetUsage
    {
        DiscardContents,
        PreserveContents
    };

    // Enum for BindFlags
    enum class BindFlags
    {
        None = 0,
        RenderTarget = 1 << 0,
        ShaderResource = 1 << 1,
        UnorderedAccess = 1 << 2
    };

    // Enum for MultiSampleCount
    enum class MultiSampleCount
    {
        None = 0,
        TwoSamples = 2,
        FourSamples = 4,
        EightSamples = 8
    };

    class RenderTarget
    {
    public:

        enum Flag
        {
            None = 0,
            DepthBuffer = 1 << 0,
            ColorBuffer = 1 << 1
        };

        RenderTarget(uint32_t width, uint32_t height, PixelFormat format)
            : Width(width), Height(height), Format(format)
        {
        }

        // Properties
        uint32_t Width = 0;
        uint32_t Height = 0;
        PixelFormat Format = PixelFormat::RGBA8;
        DepthFormat DepthFormat = DepthFormat::Depth24Stencil8;
        MultiSampleCount MultiSample = MultiSampleCount::None;
        uint32_t MipLevels = 1;
        uint32_t ArraySize = 1;
        glm::vec4 ClearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        float ClearDepth = 1.0f;
        RenderTargetUsage Usage = RenderTargetUsage::DiscardContents;
        BindFlags BindFlags = BindFlags::None;
    };
} // namespace powe
