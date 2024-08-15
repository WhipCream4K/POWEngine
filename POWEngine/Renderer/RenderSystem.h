#pragma once

#include <functional>
#include "RenderEntityIDGen.h"

namespace powe
{
    class Scene;
    class BaseRenderSystem
    {
    public:
        
        BaseRenderSystem() = default;
        BaseRenderSystem(const BaseRenderSystem&) = delete;
        BaseRenderSystem& operator=(const BaseRenderSystem&) = delete;
        BaseRenderSystem(BaseRenderSystem&&) = delete;
        BaseRenderSystem& operator=(BaseRenderSystem&&) = delete;
        virtual ~BaseRenderSystem() = default;

        virtual void Render(Scene&,const std::function<void*()>&);
        virtual uint32_t GetRenderEntityID() const = 0;
    };
    
    template <RenderState T>
    class RenderSystem : public BaseRenderSystem
    {
    public:
        
        RenderSystem() = default;
        RenderSystem(const RenderSystem&) = delete;
        RenderSystem& operator=(const RenderSystem&) = delete;
        RenderSystem(RenderSystem&&) = delete;
        RenderSystem& operator=(RenderSystem&&) = delete;
        virtual ~RenderSystem() override = default;

        void Render(Scene&, const std::function<void*()>&) override;
        virtual void OnRender(Scene& scene,const T& renderState) = 0;
        
        uint32_t GetRenderEntityID() const final;
        
    };

    template <RenderState T>
    void RenderSystem<T>::Render(Scene& scene, const std::function<void*()>& getRenderState)
    {
        T renderState{ *static_cast<T*>(getRenderState()) };
        OnRender(scene,renderState);
    }

    template <RenderState T>
    uint32_t RenderSystem<T>::GetRenderEntityID() const
    {
        return RenderEntityIDGen::Get<T>();
    }
}
