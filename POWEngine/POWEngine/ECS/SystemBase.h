#pragma once

#include <stdexcept>

// #include "SystemKeys.h"
#include "ECSSystemBackend.h"
#include "POWEngine/ECS/ECSUtils.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

namespace powe
{
    class Window;
    struct Archetype;
    class SparseComponentManager;

    class SystemBase : public ECSSystemBackend
    {
    public:
        
        SystemBase();
        SystemBase(const SystemBase&) = delete;
        SystemBase& operator=(const SystemBase&) = delete;
        SystemBase(SystemBase&&) noexcept = default;
        SystemBase& operator=(SystemBase&&) noexcept = default;
        virtual ~SystemBase() override = default;

    protected:

        void Update(const Archetype&, float) final;
        void Draw(const Window&, const Archetype&, const RenderAPI&) final {}
        
        virtual void OnUpdate(float, powe::GameObjectID) = 0;
    
    private:
        
        using ECSSystemBackend::m_CurrentArchetype;
        using ECSSystemBackend::m_UpdateCountPerArchetype;

    };

    // template <typename ComponentType>
    // EnableIsBasedOf<BaseComponent, ComponentType, ComponentType&> SystemBase::GetComponent() const
    // {
    //     return GetComponent<ComponentType>(*m_CurrentArchetype);
    // }
    //
    // template <typename ... Args>
    // std::tuple<std::add_lvalue_reference_t<Args>...> SystemBase::GetComponentsView() const
    // {
    //     return std::make_tuple(std::ref(GetComponent<Args>(*m_CurrentArchetype))...);
    // }
    //
    // template <typename T>
    // T& SystemBase::GetComponent(const Archetype& archetype) const
    // {
    //     const ComponentTypeID compID{BaseComponent::GetId<T>()};
    //     const WorldEntity& world{GetWorld()};
    //     
    //     const auto findItr = archetype.ComponentOffsets.find(compID);
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
    //             return *reinterpret_cast<T*>(dataAddress);
    //         }
    //
    //         // if it is Sparse component
    //         // get the data from sparse section
    //         auto& sparseManager{world.GetSparseComponentManager()};
    //
    //         //const SparseHandle* handle{ reinterpret_cast<SparseHandle*>(dataAddress) };
    //
    //         RawByte* realCompData{
    //             sparseManager.GetComponentData<T>(
    //                 archetype.GameObjectIds[m_UpdateCountPerArchetype], compID)
    //         };
    //
    //
    //         return *reinterpret_cast<T*>(realCompData);
    //     }
    //
    //     throw std::out_of_range(typeid(T).name());
    // }

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
