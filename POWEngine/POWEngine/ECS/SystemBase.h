#pragma once

#include <stdexcept>

#include "SystemKeys.h"
#include "POWEngine/ECS/ECSUtils.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

namespace powe
{
    struct Archetype;
    class SparseComponentManager;

    class SystemBase : public SystemKeys
    {
        friend class WorldEntity;
        friend class ThreadSystem;
    public:
        
        SystemBase();
        SystemBase(const SystemBase&) = delete;
        SystemBase& operator=(const SystemBase&) = delete;
        SystemBase(SystemBase&&) noexcept = default;
        SystemBase& operator=(SystemBase&&) noexcept = default;
        virtual ~SystemBase() = default;
    
    protected:

        WorldEntity& GetWorld() const { return *m_World; }
        
        virtual void InternalUpdate(Archetype*, float);
        virtual void InternalCreate(WorldEntity* world,const Archetype&);
        void InternalDestroy(const Archetype&);

        virtual void OnUpdate(float, powe::GameObjectID) = 0;

        /**
         * \brief Run once after all the elements have been iterate through
         */
        virtual void OnPreCreate() {}
        
        virtual void OnCreate(GameObjectID) {}

        virtual void OnDestroy(GameObjectID) {}

        // Specialize GetComponent from iteration
        template <typename ComponentType>
        EnableIsBasedOf<BaseComponent, ComponentType, ComponentType&> GetComponent() const;

        // Specialize GetComponent from iteration
        // template <typename ...Args>
        // std::tuple<std::add_pointer_t<Args>...> GetComponentsView() const;

        template<typename ...Args>
        std::tuple<std::add_lvalue_reference_t<Args>...> GetComponentsView() const;
        
        RawByte* GetComponentAddress(ComponentTypeID id,SizeType componentSize) const;
    
    private:
        
        template<typename T>
        T& GetComponent(const Archetype& archetype) const;

        RawByte* GetComponentAddressByID(const Archetype& archetype,ComponentTypeID id,SizeType componentSize) const;
        
        // template<typename T>
        // T* InternGetComponentByID(const Archetype& archetype,ComponentTypeID id) const;
        
        WorldEntity* m_World;
        
        // using pointer to be more flexible. Doesn't really need smartpointer here there's no ownership changing anyway
        const Archetype* m_CurrentArchetype;
        uint32_t m_UpdateCountPerArchetype;
    };

    template <typename ComponentType>
    EnableIsBasedOf<BaseComponent, ComponentType, ComponentType&> SystemBase::GetComponent() const
    {
        return GetComponent<ComponentType>(*m_CurrentArchetype);
    }

    template <typename ... Args>
    std::tuple<std::add_lvalue_reference_t<Args>...> SystemBase::GetComponentsView() const
    {
        return std::make_tuple(std::ref(GetComponent<Args>(*m_CurrentArchetype))...);
    }

    // template <typename ... Args>
    // std::tuple<std::add_pointer_t<Args>...> SystemBase::GetComponentsView() const
    // {
    //     return std::make_tuple(GetComponent<Args>(*m_CurrentArchetype)...);
    // }

    // template <typename T>
    // T& SystemBase::GetComponentByID(ComponentTypeID id) const
    // {
    //     return InternGetComponentByID<T>(*m_CurrentArchetype,id);
    // }

    template <typename T>
    T& SystemBase::GetComponent(const Archetype& archetype) const
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
                        m_UpdateCountPerArchetype * archetype.SizeOfComponentsBlock
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
                    archetype.GameObjectIds[m_UpdateCountPerArchetype], compID)
            };


            return *reinterpret_cast<T*>(realCompData);
        }

        throw std::out_of_range(typeid(T).name());
    }

    // template <typename T>
    // T* SystemBase::InternGetComponentByID(const Archetype& archetype, ComponentTypeID id) const
    // {
    //     const auto findItr = archetype.ComponentOffsets.find(id);
    //
    //     if (findItr != archetype.ComponentOffsets.end())
    //     {
    //         // check the key if it's a sparse component or not
    //         if (!IsThisComponentSparse(findItr->first))
    //         {
    //             RawByte* dataAddress{
    //                 &archetype.ComponentData[
    //                     m_UpdateCountPerArchetype * archetype.SizeOfComponentsBlock
    //                     + findItr->second // offsets
    //                 ]
    //             };
    //             
    //             return reinterpret_cast<T*>(dataAddress);
    //         }
    //
    //         // if it is Sparse component
    //         // get the data from sparse section
    //         auto& sparseManager{m_World->GetSparseComponentManager()};
    //         
    //         RawByte* realCompData{
    //             sparseManager.GetComponentData<T>(
    //                 archetype.GameObjectIds[m_UpdateCountPerArchetype], id)
    //         };
    //         
    //         return reinterpret_cast<T*>(realCompData);
    //     }
    //
    //     throw std::out_of_range(typeid(T).name());
    // }
}
