#pragma once
#include "SystemBase.h"

namespace powe
{

    class ThreadExecute
    {
        friend class SystemBase;
        
    public:
        
        ThreadExecute() = default;
        
        bool IsFinished() const;
        void OnCreate(WorldEntity* world, const Archetype*,uint32_t);

    protected:
        
        void SetTaskState(bool state);
        
        template<typename ...Args>
        std::tuple<std::add_lvalue_reference_t<Args>...> GetComponentsView() const;
        
    private:

        template<typename T>
        T& GetComponent(const Archetype& archetype) const;
    
        const Archetype* m_CurrentArchetype{};
        WorldEntity* m_World{};
        uint32_t m_UpdateIndex{};
        std::atomic_bool m_TaskFinished{};
    };

    template <typename ... Args>
    std::tuple<std::add_lvalue_reference_t<Args>...> ThreadExecute::GetComponentsView() const
    {
        return std::make_tuple(std::ref(GetComponent<Args>(*m_CurrentArchetype))...);
    }

    template <typename T>
    T& ThreadExecute::GetComponent(const Archetype& archetype) const
    {
        const ComponentTypeID compID{BaseComponent::GetId<T>()};

        const auto findItr = archetype.ComponentOffsets.find(compID);

        if (findItr != archetype.ComponentOffsets.end())
        {
            // check the key if it's a sparse component or not
            if (!IsThisComponentSparse(findItr->first))
            {
                RawByte* dataAddress{
                    &archetype.ComponentData[
                        m_UpdateIndex * archetype.SizeOfComponentsBlock
                        + findItr->second // offsets
                    ]
                };
                
                return *reinterpret_cast<T*>(dataAddress);
            }

            // if it is Sparse component
            // get the data from sparse section
            auto& sparseManager{m_World->GetSparseComponentManager()};

            //const SparseHandle* handle{ reinterpret_cast<SparseHandle*>(dataAddress) };

            RawByte* realCompData{
                sparseManager.GetComponentData<T>(
                    archetype.GameObjectIds[m_UpdateIndex], compID)
            };


            return *reinterpret_cast<T*>(realCompData);
        }

        throw std::out_of_range(typeid(T).name());
    }
}

