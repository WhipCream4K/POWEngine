#pragma once

#include <vector>
#include <functional>

#include "ComponentInfo.h"
#include "ECSTypes.h"
#include "Core/CustomTypes.h"
#include "Core/Memory/AllocatorContext.h"

namespace powe
{
    using UniqueBlockPtr = UniquePtr<void, std::function<void(void*)>>;

    class IArchetype
    {
    public:
    
        IArchetype() = default;
        IArchetype(const IArchetype&) = default;
        IArchetype& operator=(const IArchetype&) = default;
        IArchetype(IArchetype&&) = default;
        IArchetype& operator=(IArchetype&&) = default;
        virtual ~IArchetype() = default;

        virtual void GetComponents(const Vector<ComponentID>& query, Vector<CompAddress>& outAddress) const = 0;
        virtual Vector<ComponentID> GetComponentIDs() const = 0;
        virtual Set<ComponentID> GetComponentIDSet() const = 0;
        virtual CompAddress GetFrontComponentAddress() const = 0;

        // Remove entity from archetype and return the tuple of removed components
        // Useful for moving entities to another archetype
        virtual constexpr void Remove(EntityID id, UniqueBlockPtr& outBlock) noexcept = 0;
        virtual constexpr void Remove(EntityID id, ComponentStorage& outComponents) noexcept = 0;

        virtual constexpr void Move(EntityID id,const Vector<std::pair<ComponentID,CompAddress>>& movedComponents) noexcept = 0;
        virtual constexpr bool HasComponent(const Vector<ComponentID>& query) const noexcept = 0;

        virtual constexpr size_t size() const noexcept = 0;
        virtual constexpr bool contains(EntityID id) const noexcept = 0;
        virtual constexpr bool empty() const noexcept = 0;


        virtual void AddArchetypeInvalidCallback(std::function<void(IArchetype*)> callback) = 0;
    };

    template <ComponentConcept... Args>
    class Archetype final : public IArchetype
    {
        using InvalidCallback = std::function<void(IArchetype*)>;
        using ComponentBlock = std::tuple<Args...>;

    private:

        void CreateTupleIndex(Vector<ComponentID>& tupleIndex) const
        {
            tupleIndex.reserve(sizeof...(Args));
            ((tupleIndex.emplace_back(ComponentInfo::GetIDID<Args>()), ...));
        }

        Vector<std::pair<ComponentID,CompAddress>> SortMoveBlock(const Vector<std::pair<ComponentID,CompAddress>>& movedComponents) const
        {
            Vector<std::pair<ComponentID,CompAddress>> sortedMovedComponents{ movedComponents };

            // Define a comparison function based on m_ComponentToTupleIndex
            // for example, moveedComponents = {{2,nullptr},{1,nullptr},{0,nullptr}} m_ComponentToTupleIndex = {0,1,2}
            // then sortedMovedComponents = {{0,nullptr},{1,nullptr},{2,nullptr}}
            auto compare = [this](const std::pair<ComponentID, CompAddress>& lhs, const std::pair<ComponentID, CompAddress>& rhs) {
                auto lhsIt = std::find(m_ComponentToTupleIndex.begin(), m_ComponentToTupleIndex.end(), lhs.first);
                auto rhsIt = std::find(m_ComponentToTupleIndex.begin(), m_ComponentToTupleIndex.end(), rhs.first);
                return lhsIt < rhsIt;
            };

            std::sort(sortedMovedComponents.begin(), sortedMovedComponents.end(), compare);

            return sortedMovedComponents;
        }

        template<size_t... Is>
        ComponentBlock MakeBlockFrom(const Vector<std::pair<ComponentID,CompAddress>>& movedComponents, std::index_sequence<Is...>) const
        {
            return std::make_tuple(*static_cast<Args*>(movedComponents[Is].second)...);
        }

    public:

        Archetype()
        {
            const AllocatorContext context{ AllocatorScope::Game };
            auto* upStream{ context.GetResource() };
            
            m_ComponentToTupleIndex = Vector<ComponentID>(upStream);
            m_Components = Vector<ComponentBlock>(upStream);
            m_EntityToIndex = UnOrderedMap<EntityID, uint32_t>(upStream);
            m_InvalidCallbacks = Vector<InvalidCallback>(upStream);

            CreateTupleIndex(m_ComponentToTupleIndex);
        }

        void GetComponents(const Vector<ComponentID>& query, Vector<CompAddress>& outAddress) const override
        {
            const size_t blockCount{ m_Components.size() };
            outAddress.resize(query.size() * blockCount, nullptr);

            for(size_t blockIdx = 0; blockIdx < blockCount; ++blockIdx) {
                auto& block = m_Components[blockIdx];
                std::apply([&outAddress, &query, blockIdx](auto&&... components) {
                    for (size_t i = 0; i < query.size(); ++i) {
                        size_t outIdx = blockIdx * query.size() + i;
                        ((ComponentInfo::GetID<decltype(components)>() == query[i] ? 
                          outAddress[outIdx] = &components : nullptr), ...);
                    }
                }, block);
            }
        }

        void* GetFrontComponentAddress() const override
        {
            return m_Components.empty() ? nullptr : &m_Components.front();
        }

        void Move(EntityID id, const Vector<std::pair<ComponentID,CompAddress>>& movedComponents) noexcept override
        {
            auto sortedMovedComponents{ SortMoveBlock(movedComponents)};
            auto newBlock{ MakeBlockFrom(sortedMovedComponents, std::make_index_sequence<sizeof...(Args)>{}) };

            emplace_back(id, std::move(newBlock));
        }

        void emplace_back(EntityID id, ComponentBlock&& block)
        {
            m_Components.emplace_back(std::move(block));
            m_EntityToIndex[id] = m_Components.size() - 1;

            // TODO: Add call_back invalidation
        }

        void Remove(EntityID id, UniqueBlockPtr& outBlock) noexcept override
        {
            if(auto findItr{ m_EntityToIndex.find(id)}; findItr != m_EntityToIndex.end())
            {
                if(outBlock)
                {
                    auto& block = m_Components[findItr->second];

                    // Making a unique_ptr to avoid deletion of the block
                    outBlock = std::unique_ptr<void, std::function<void(void*)>>(
                        new ComponentBlock(std::move(block)),
                        [](void* ptr) {
                            delete static_cast<ComponentBlock*>(ptr);
                        }
                    );
                }

                m_Components.erase(m_Components.begin() + findItr->second);
                m_EntityToIndex.erase(findItr);

                // TODO: Add call_back invalidation
            }

        }

        void Remove(EntityID id, ComponentStorage &outComponents) noexcept override
        {
            if(auto findItr{ m_EntityToIndex.find(id)}; findItr != m_EntityToIndex.end())
            {
                // pre allocate memory for emplacement
                outComponents.resize(sizeof...(Args));

                ComponentBlock& block = m_Components[findItr->second];

                std::apply([&outComponents](auto&&... component) {

                    
                    

                }, block);

                m_Components.erase(m_Components.begin() + findItr->second);
                m_EntityToIndex.erase(findItr);
            }

            // TODO: Add call_back invalidation
        }

        bool HasComponent(const Vector<ComponentID> &query) const override
        {
            // TODO: Finish this function
        }

        size_t size() const noexcept override
        {
            return m_Components.size();
        }

        bool contains(EntityID id) const noexcept override
        {
            return m_EntityToIndex.contains(id);
        }

        bool empty() const noexcept override
        {
            return m_Components.empty();
        }

    private:
    
        Vector<ComponentBlock> m_Components;

        // Run-time tuple index
        Vector<ComponentID> m_ComponentToTupleIndex;

        UnOrderedMap<EntityID, uint32_t> m_EntityToIndex;
        Vector<InvalidCallback> m_InvalidCallbacks;
    };
}
