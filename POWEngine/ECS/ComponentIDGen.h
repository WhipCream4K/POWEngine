#pragma once

#include "ECSTypes.h"

namespace powe
{
    // Template class for generating unique ID for each component type
    class ComponentIDGen final
    {
    
    public:

        template<ComponentConcept Component>
        static ComponentID Get()
        {
            static const ComponentID id{GenerateID()};
            return id;
        }

        static size_t Size()
        {
            return m_Counter;
        }

    private:
        
        static ComponentID GenerateID()
        {
            return m_Counter++;
        }

        static std::atomic<ComponentID> m_Counter;
        
    };    
}

