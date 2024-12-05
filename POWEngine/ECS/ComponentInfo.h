#pragma once

#include "ECSTypes.h"

namespace powe
{
    // Template class for generating unique ID for each component type
    class ComponentInfo final
    {
    
    public:

        template<ComponentConcept Component>
        static ComponentID GetID() noexcept
        {
            static const ComponentID id{GenerateID()};

            // thread unsafe
            if(!IsRegistered(id))
                Register<Component>(id);

            return id;
        }

        // Will throw exception if component is not call by GetID first, which is unlikely
        static size_t GetSize(ComponentID id)
        {
            return m_IDToComponentSize.at(id);
        }  

        static std::function<void(void*,void*)> GetMoveOp(ComponentID id)
        {
            return m_IDToMoveOp.at(id);
        }

        static std::function<void(void*)> GetDestroyOp(ComponentID id)
        {
            return m_IDToDestroyOp.at(id);
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

        static bool IsRegistered(ComponentID id) noexcept
        {
            return m_IDToComponentSize.find(id) != m_IDToComponentSize.end();
        }

        template<ComponentConcept Component>
        static void Register(ComponentID id)
        {
            m_IDToComponentSize[id] = sizeof(Component);
        
            m_IDToMoveOp[id] = [](void* src, void* dst) { 
                new (dst) Component(std::move(*static_cast<Component*>(src)));
                 };
        
            m_IDToDestroyOp[id] = [](void* ptr) {
                static_cast<Component*>(ptr)->~Component();
            };
        }

        static std::atomic<ComponentID> m_Counter;
        static UnOrderedMap<ComponentID,size_t> m_IDToComponentSize;
        static UnOrderedMap<ComponentID,std::function<void(void*,void*)>> m_IDToMoveOp;
        static UnOrderedMap<ComponentID,std::function<void(void*)>> m_IDToDestroyOp;
        
    };    
}

