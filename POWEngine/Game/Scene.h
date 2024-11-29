#pragma once


#include "SceneSystem.h"
#include "Utils/Utils.h"
// #include "SceneQuery.h"
#include "Game/Input/InputManager.h"

namespace powe
{
    class ECSManager;
    class EngineLayer;
    class Game;
    class Scene final
    {
    public:
        
        Scene(Game& game);

        void Start();
        void Exit();
        void Update(float deltaTime);

        template <CSceneSystem T>
        T* AddSceneSystem(T&& system)
        {
            auto sceneSystem = AllocateUnique<T>(m_DefaultAllocator, std::forward<T>(system));
            m_SceneSystems.push_back(std::move(sceneSystem));
            return static_cast<T*>(m_SceneSystems.back().get());
        }

        template <CSceneSystem T> 
        void RemoveSceneSystem()
        {
            std::erase_if(m_SceneSystems, [](const UniquePtr<SceneSystem>& sceneSystem)
            {
                return typeid(*sceneSystem.get()) == typeid(T);
            });
        }
        
        ECSManager& GetECSManager() const { return *m_ECSManager.get(); }
        Game& GetGameModule() const noexcept { return m_Game; }
        std::string_view GetName() const noexcept;

    private:
        
        UniquePtr<ECSManager> m_ECSManager;
        RefWrap<Game> m_Game;
        InputManager m_InputManager;
        Vector<UniquePtr<SceneSystem>> m_SceneSystems;
        
    };
}
