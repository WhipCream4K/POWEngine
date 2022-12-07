#pragma once
#include "POWEngine/Core/Components/BaseComponent.h"
#include "ECSTypes.h"

namespace powe
{
    struct Archetype;
    class WorldEntity;
    class ECSSystemBackend
    {
    public:

        ECSSystemBackend() = default;
        virtual ~ECSSystemBackend() = default;

        void Initialize(WorldEntity& worldEntity);
        
        virtual void Update(const Archetype&, float) = 0;
        virtual void OnCreate() {}
        virtual void Destroy(const Archetype&) {}

        [[nodiscard]] const std::unordered_set<ComponentTypeID>& GetKeys() const {return m_Keys;}
        
        template <typename ...Args, typename = std::enable_if_t<(std::is_base_of_v<BaseComponent, Args> && ...)>>
        void DefineSystemKeys();

    protected:
        
        /**
         * \brief GetWorld should be valid at all times if somehow world pointer is null then
         * this should throw
         * \return WorldEntity that this system is registering in
         */
        WorldEntity& GetWorld() const {return *m_World;}
        
    private:

        std::unordered_set<ComponentTypeID> m_Keys{};
        WorldEntity* m_World{};
    };

    template <typename ... Args, typename>
    void ECSSystemBackend::DefineSystemKeys()
    {
        m_Keys = {BaseComponent::GetId<Args>()...};
    }
    
}
