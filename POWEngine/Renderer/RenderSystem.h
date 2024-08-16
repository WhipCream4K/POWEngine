#pragma once

#include <functional>
#include "RenderResourceIDGen.h"

namespace powe
{
    class Scene;
    class BaseRenderSystem
    {
    public:
        
        BaseRenderSystem() = default;
        BaseRenderSystem(const BaseRenderSystem&) = default;
        BaseRenderSystem& operator=(const BaseRenderSystem&) = default;
        BaseRenderSystem(BaseRenderSystem&&) = default;
        BaseRenderSystem& operator=(BaseRenderSystem&&) = default;
        virtual ~BaseRenderSystem() = default;

        virtual void Render(Scene&,const std::function<void*()>&);
        virtual uint32_t GetRenderEntityID() const = 0;
    };
    
    template <CRenderResource T>
    class RenderSystem : public BaseRenderSystem
    {
    public:
        
        RenderSystem() = default;
        RenderSystem(const RenderSystem&) = default;
        RenderSystem& operator=(const RenderSystem&) = default;
        RenderSystem(RenderSystem&&) noexcept = default;
        RenderSystem& operator=(RenderSystem&&) noexcept = default;
        virtual ~RenderSystem() override = default;

        void Render(Scene&, const std::function<void*()>&) override;
        virtual void OnRender(Scene& scene,const T& renderState) = 0;
        
        uint32_t GetRenderEntityID() const final;
        
    };

    template <CRenderResource T>
    void RenderSystem<T>::Render(Scene& scene, const std::function<void*()>& getRenderState)
    {
        T renderState{ *static_cast<T*>(getRenderState()) };
        OnRender(scene,renderState);
    }

    template <CRenderResource T>
    uint32_t RenderSystem<T>::GetRenderEntityID() const
    {
        return RenderResourceIDGen::Get<T>();
    }
}
