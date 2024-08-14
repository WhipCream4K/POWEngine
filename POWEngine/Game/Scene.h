#pragma once

#include <typeindex>

#include "EngineLayer.h"
#include "SceneSystem.h"
#include "Utils/Utils.h"
#include "SceneQuery.h"
#include "InputManager.h"

namespace powe
{
    class ECSManager;
    class EngineLayer;

    class Scene final
    {
    public:
        Scene(EngineLayer& parent);

        void OnStart();
        void OnExit();
        void OnWindowEvents(const Window::EventQueue& winEvents) const;
        void Update(float deltaTime);

        template <typename T> requires CSceneSystem<T>
        T* AddSceneSystem(T&& system)
        {
            auto sceneSystem = AllocateUnique<T>(GetParentLayer().GetAllocator(), std::move(system));
            m_SceneSystems.push_back(std::move(sceneSystem));
            return static_cast<T*>(m_SceneSystems.back().get());
        }

        template <typename T> requires CSceneSystem<T>
        void RemoveSceneSystem()
        {
            std::erase_if(m_SceneSystems, [](const UniquePtr<SceneSystem>& sceneSystem)
            {
                return typeid(*sceneSystem.get()) == typeid(T);
            });
        }


        // EngineLayer is guaranteed to be valid as long as Scene Exists
        EngineLayer& GetParentLayer() const { return m_EngineLayer; }
        InputManager& GetInputManager() const {return *m_InputManager.get();}
        ECSManager& GetECSManager() const { return *m_ECSManager.get(); }
        const std::string& GetName() const { return m_SceneName; }

        /**
         * Query components from the scene that can be used to iterate over the components
         * @tparam Args Components to query
         * @return iterator to the components
         */
        template<typename... Args> requires (ComponentConcept<Args> && ...)
        constexpr ComponentCollection<Args...> Query()
        {
            return SceneQuery::QueryComponents<Args...>(m_CachedComponentViews, m_CacheQueryIDs);
        }

    private:
        
        UniquePtr<ECSManager> m_ECSManager;
        UniquePtr<InputManager> m_InputManager;
        RefWrap<EngineLayer> m_EngineLayer;
        Vector<UniquePtr<SceneSystem>> m_SceneSystems;
        UnOrderedMap<std::type_index, SharedPtr<void>> m_SubSystems;
        
        // For query caching
        DynamicBitsetRange<ComponentView> m_CachedComponentViews;
        Vector<ComponentID> m_CacheQueryIDs;

        std::string m_SceneName;
        
    };
}
