#pragma once
#include "Core.h"
#include "POWEngine/Renderer/RenderAPI.h"
#include "POWEngine/Renderer/Renderer.h"

namespace powe
{
    class Core;
    class CoreResource
    {
    public:

        CoreResource(const SharedPtr<Core>& core);

        template<typename T>
        EnableIsBasedOf<RenderAPI,T,T&> GetRenderAPI() const;
        
    private:
        
        // all core accessors need core to be alive until it escapes their scopes
        WeakPtr<Core> m_Core;
    };

    template <typename T>
    EnableIsBasedOf<RenderAPI, T, T&> CoreResource::GetRenderAPI() const
    {
        if(const SharedPtr<Core> coreReal{m_Core.lock()})
        {
            return static_cast<T&>(*coreReal->GetRenderer().GetRenderAPI());
        }

        throw std::runtime_error("Core doesn't exist in caller scope");
    }
}


