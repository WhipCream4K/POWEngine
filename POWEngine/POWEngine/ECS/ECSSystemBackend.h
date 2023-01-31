#pragma once
#include "Archetype.h"
#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "ECSTypes.h"
#include "ECSUtils.h"


namespace powe
{
    class Window;
    struct Archetype;
    class WorldEntity;
    class RenderAPI;

    class ECSSystemBackend
    {
    public:
        ECSSystemBackend() = default;
        virtual ~ECSSystemBackend() = default;
        
        virtual void Initialize(WorldEntity& worldEntity, const Archetype&);
        virtual void Update(const Archetype&, float) = 0;
        virtual void Draw(const Window&, const Archetype&, const RenderAPI&) = 0;
        virtual void Destroy(const Archetype&);

        
        virtual void OnDestroy(GameObjectID) {}
        virtual void OnCreate() {}
        virtual void OnPerGameObjectCreate(powe::GameObjectID) {}

        [[nodiscard]] const std::unordered_set<ComponentTypeID>& GetKeys() const { return m_Keys; }

        template <typename... Args, typename = std::enable_if_t<(std::is_base_of_v<BaseComponent, Args> && ...)>>
        void DefineSystemKeys();

    protected:
        /**
         * \brief GetWorld should be valid at all times if somehow world pointer is null then
         * this should throw
         * \return WorldEntity that this system is registering in
         */
        WorldEntity& GetWorld() const { return *m_World; }


        // Specialize GetComponent from iteration
        template <typename ComponentType>
        EnableIsBasedOf<BaseComponent, ComponentType, ComponentType&> GetComponent() const;

        template <typename... Args>
        std::tuple<std::add_lvalue_reference_t<Args>...> GetComponentsView() const;

        RawByte* GetComponentAddress(ComponentTypeID id, SizeType componentSize) const;


        // using pointer to be more flexible. Doesn't really need smartpointer here there's no ownership changing anyway
        const Archetype* m_CurrentArchetype;
        uint32_t m_UpdateCountPerArchetype;
        
    private:
        
        template <typename T>
        T& GetComponent(const Archetype& archetype) const;

        RawByte* GetComponentAddressByID(const Archetype& archetype, ComponentTypeID id, SizeType componentSize) const;

        std::unordered_set<ComponentTypeID> m_Keys{};
        WorldEntity* m_World{};


    };

    template <typename... Args, typename>
    void ECSSystemBackend::DefineSystemKeys()
    {
        m_Keys = {BaseComponent::GetId<Args>()...};
    }


    template <typename ComponentType>
    EnableIsBasedOf<BaseComponent, ComponentType, ComponentType&> ECSSystemBackend::GetComponent() const
    {
        return GetComponent<ComponentType>(*m_CurrentArchetype);
    }

    template <typename... Args>
    std::tuple<std::add_lvalue_reference_t<Args>...> ECSSystemBackend::GetComponentsView() const
    {
        return std::make_tuple(std::ref(GetComponent<Args>(*m_CurrentArchetype))...);
    }

    template <typename T>
    T& ECSSystemBackend::GetComponent(const Archetype& archetype) const
    {
        const ComponentTypeID compID{BaseComponent::GetId<T>()};
        // const WorldEntity& world{GetWorld()};

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
}
