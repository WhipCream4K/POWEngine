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
        
        Scene(PMRResource* memResource);

        void OnStart();
        void OnExit();
        // void OnWindowEvents(const Window::EventQueue& winEvents) const;
        void Update(float deltaTime);

        template <typename T> requires CSceneSystem<T>
        T* AddSceneSystem(T&& system)
        {
            auto sceneSystem = AllocateUnique<T>(m_DefaultAllocator, std::forward<T>(system));
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
        
        ECSManager& GetECSManager() const { return *m_ECSManager.get(); }
        const std::string& GetName() const { return m_SceneName; }

    private:
        
        std::string m_SceneName;
        UniquePtr<ECSManager> m_ECSManager;
        
        Vector<UniquePtr<SceneSystem>> m_SceneSystems;
        
        SceneQuery m_SceneQuery;


        PMRResource* m_DefaultAllocator;
        
    };
}
