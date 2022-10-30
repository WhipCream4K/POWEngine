#pragma once
#include "ECSTypes.h"
#include "POWEngine/Core/Components/BaseComponent.h"

namespace powe
{
    class SystemBase;
    class SystemKeys
    {
    public:

        [[nodiscard]] const std::unordered_set<ComponentTypeID>& GetKeys() const {return m_Keys;}
        
        template <typename ...Args, typename = std::enable_if_t<(std::is_base_of_v<Component<Args>, Args> && ...)>>
        void DefineSystemKeys();
    
    private:
        
        std::unordered_set<ComponentTypeID> m_Keys;
    };

    template <typename ... Args, typename>
    void SystemKeys::DefineSystemKeys()
    {
        m_Keys = {BaseComponent::GetId<Args>()...};
    }
}


