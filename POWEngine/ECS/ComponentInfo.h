#pragma once

#include "ECSTypes.h"

namespace powe
{
    // Template class for generating unique ID for each component type
    class ComponentInfo final
    {
    
    public:

        template<ComponentConcept Component>
        static ComponentID GetID()
        {
            static const ComponentID id{GenerateID()};

            // thread unsafe
            if(m_IDToComponentSize.find(id) == m_IDToComponentSize.end())
            {
                m_IDToComponentSize[id] = sizeof(Component);
            }

            return id;
        }

        template<ComponentConcept Component>
        static size_t GetSize()
        {
            return m_IDToComponentSize.at(GetID<Component>());
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
        static UnOrderedMap<ComponentID,size_t> m_IDToComponentSize;
        
    };    
}

