#pragma once
#include "RenderAPI.h"
#include "POWEngine/ECS/ECSTypes.h"

namespace powe
{
    class Window;
    
    class RenderCommandBase
    {
    public:
        
        RenderCommandBase() = default;
        RenderCommandBase(const RenderCommandBase&) = delete;
        RenderCommandBase& operator=(const RenderCommandBase&) = delete;
        RenderCommandBase(RenderCommandBase&&) noexcept = delete; 
        RenderCommandBase& operator=(RenderCommandBase&&) noexcept = delete;
        virtual ~RenderCommandBase() = default;
        
        virtual void Render(const Window& renderWindow,const RenderAPI& renderer) = 0;

        // virtual SizeType GetSize() const = 0;
    };

    template <bool U>
    class IfBase
    {
    };

    template <>
    class IfBase<false> final
    {
    };

    template <>
    class IfBase<true>
    {
    };
    
    template<typename T,typename RendererType>
    class RenderCommand : public RenderCommandBase,IfBase<std::is_base_of_v<RenderAPI, RendererType>>
    {
    public:
        
        RenderCommand() = default;
        virtual ~RenderCommand() override = default;
        void Render(const Window& renderWindow, const RenderAPI& renderer) override;
        // SizeType GetSize() const override;
        virtual void OnRender(const Window& renderWindow,const T& renderer) = 0;
    };

    template <typename T, typename RendererType>
    void RenderCommand<T, RendererType>::Render(const Window& renderWindow, const RenderAPI& renderer)
    {
        OnRender(renderWindow,static_cast<const RenderAPI&>(renderer));
    }
}
