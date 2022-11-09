#pragma once

#include "POWEngine/ECS/SystemBase.h"
#include "POWEngine/Logger/LoggerUtils.h"
// #include "POWEngine/Window/Window.h"

namespace powe
{
    class Renderer;
    class RenderAPI;
    class Window;

    class RenderSystemBase : public SystemKeys
    {
    public:
        
        virtual void InternalDraw(const WorldEntity& worldEntity,
            const Window& renderWindow,const Archetype&, const RenderAPI&) = 0;

        RenderSystemBase() = default;
        RenderSystemBase(const RenderSystemBase&) = delete;
        RenderSystemBase& operator=(const RenderSystemBase&) = delete;
        RenderSystemBase(RenderSystemBase&&) = delete;
        RenderSystemBase& operator=(RenderSystemBase&&) = delete;
        virtual ~RenderSystemBase() = default;
    };

    template <bool U>
    class IfBase
    {
    };

    template <>
    class IfBase<false> final
    {
    };

    template <>
    class IfBase<true>
    {
    };

    template <typename T>
    class RenderSystem : public RenderSystemBase, public IfBase<std::is_base_of_v<RenderAPI, T>>
    {
    public:
        
        void InternalDraw(const WorldEntity& worldEntity,
            const Window& renderWindow,
            const Archetype&, const RenderAPI&) final;
        
        virtual void OnDraw(const T& renderer,const Window& renderWindow, GameObjectID id) = 0;

    public:
        RenderSystem() = default;
        virtual ~RenderSystem() override = default;

    protected:
        
        const WorldEntity* GetWorld() const { return m_CurrentWorld; }

        // Specialize GetComponent from iteration
        template <typename ComponentType>
        EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GetComponent() const;

        template <typename U>
        U* GetComponentByID(ComponentTypeID id) const;

        RawByte* GetComponentAddress(ComponentTypeID id,SizeType size) const;

        // Specialize GetComponent from iteration
        template <typename ...Args>
        std::tuple<std::add_pointer_t<Args>...> GetComponentsView() const;


    private:
        
        template <typename U>
        U* GetComponent(const Archetype& archetype) const;

        template <typename U>
        U* GetComponentFromID(const Archetype& archetype, ComponentTypeID id) const;

        RawByte* GetComponentAddress(const Archetype& archetype,ComponentTypeID id,SizeType size) const;

    private:
        const Archetype* m_CurrentArchetype{};
        const WorldEntity* m_CurrentWorld{};
        uint32_t m_UpdateCountPerArchetype{};
    };

    template <typename T>
    void RenderSystem<T>::InternalDraw(const WorldEntity& worldEntity,
        const Window& renderWindow,
        const Archetype& archetype,
        const RenderAPI& renderer)
    {
        m_CurrentArchetype = &archetype;
        m_UpdateCountPerArchetype = 0;
        m_CurrentWorld = &worldEntity;

        try
        {
            for (const GameObjectID gameObjectID : archetype.GameObjectIds)
            {
                OnDraw(static_cast<const T&>(renderer),renderWindow, gameObjectID);
                ++m_UpdateCountPerArchetype;
            }
        }
        catch (const std::exception& e)
        {
            std::string errMsg{"System trying to access component that doesn't exist in this archetype -> "};
            errMsg.append(e.what());
            POWLOGWARNING(errMsg);

            m_UpdateCountPerArchetype = 0;
            m_CurrentArchetype = nullptr;
            m_CurrentWorld = nullptr;
        }

        m_UpdateCountPerArchetype = 0;
        m_CurrentArchetype = nullptr;
        m_CurrentWorld = nullptr;
    }

    template <typename T>
    template <typename ... Args>
    std::tuple<std::add_pointer_t<Args>...> RenderSystem<T>::GetComponentsView() const
    {
        return std::make_tuple(GetComponent<Args>(*m_CurrentArchetype)...);
    }

    template <typename T>
    template <typename ComponentType>
    EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> RenderSystem<T>::GetComponent() const
    {
        return GetComponent<ComponentType>(*m_CurrentArchetype);
    }

    template <typename T>
    template <typename U>
    U* RenderSystem<T>::GetComponentByID(ComponentTypeID id) const
    {
        return GetComponentFromID<U>(*m_CurrentArchetype,id);
    }

    template <typename T>
    RawByte* RenderSystem<T>::GetComponentAddress(ComponentTypeID id, SizeType size) const
    {
        return GetComponentAddress(*m_CurrentArchetype,id,size);
    }

    template <typename T>
    template <typename U>
    U* RenderSystem<T>::GetComponent(const Archetype& archetype) const
    {
        const ComponentTypeID compID{BaseComponent::GetId<U>()};

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
                return reinterpret_cast<U*>(dataAddress);
            }

            // if it is Sparse component
            // get the data from sparse section
            auto& sparseManager{m_CurrentWorld->GetSparseComponentManager()};

            RawByte* realCompData{
                sparseManager.GetComponentData<U>(
                    archetype.GameObjectIds[m_UpdateCountPerArchetype], compID)
            };


            return reinterpret_cast<U*>(realCompData);
        }

        throw std::out_of_range(typeid(U).name());
    }

    template <typename T>
    template <typename U>
    U* RenderSystem<T>::GetComponentFromID(const Archetype& archetype, ComponentTypeID id) const
    {
        const auto findItr = archetype.ComponentOffsets.find(id);

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
                return reinterpret_cast<U*>(dataAddress);
            }

            // if it is Sparse component
            // get the data from sparse section
            auto& sparseManager{m_CurrentWorld->GetSparseComponentManager()};

            // RawByte* realCompData{
            //     sparseManager.GetComponentData<U>(
            //         archetype.GameObjectIds[m_UpdateCountPerArchetype], id)
            // };

            const SharedPtr<BaseComponent> baseComp{m_CurrentWorld->GetComponentTrait(id)};
            
            RawByte* realCompData{sparseManager.GetComponentData(
                archetype.GameObjectIds[m_UpdateCountPerArchetype],id,baseComp->GetSize())};

            return reinterpret_cast<U*>(realCompData);
        }

        throw std::out_of_range(typeid(U).name());
    }

    template <typename T>
    RawByte* RenderSystem<T>::GetComponentAddress(const Archetype& archetype, ComponentTypeID id, SizeType size) const
    {
        const auto findItr = archetype.ComponentOffsets.find(id);

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
                return dataAddress;
            }

            // if it is Sparse component
            // get the data from sparse section
            auto& sparseManager{m_CurrentWorld->GetSparseComponentManager()};

            // RawByte* realCompData{
            //     sparseManager.GetComponentData<U>(
            //         archetype.GameObjectIds[m_UpdateCountPerArchetype], id)
            // };
            
            RawByte* realCompData{sparseManager.GetComponentData(
                archetype.GameObjectIds[m_UpdateCountPerArchetype],id,size)};

            return realCompData;
        }

        throw std::out_of_range("Can't find this component type via GetComponentAddress()");
    }
}


