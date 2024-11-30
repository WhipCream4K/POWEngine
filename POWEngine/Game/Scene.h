#pragma once

#include "SceneSystem.h"
#include "Utils/Utils.h"
#include "Game/Input/InputManager.h"

namespace powe
{
    class ECSManager;
    class EngineLayer;
    class Game;
    class SceneEvent;
    class Scene final
    {
    public:
        
        Scene(Game& game);

        void Start();
        void Exit();
        void Update(float deltaTime);

        template<typename T,typename... Args>
        requires std::is_base_of_v<SceneEvent, T>
        void SetSceneEvent(Args&&... args)
        {
            const auto gameResource{GetResource()};
            m_SceneEvent = AllocateUnique<T>(gameResource,std::forward<Args>(args)...);
            CallCreateSceneEvent(m_SceneEvent.get());
        }

        InputManager& GetInputManager() noexcept { return m_InputManager; }
        ECSManager& GetECSManager() const { return *m_ECSManager.get(); }
        Game& GetGameModule() const noexcept { return m_Game; }
        std::string_view GetName() const noexcept;
        SharedPtr<PMRResource> GetResource() const noexcept;

    private:

        void CallCreateSceneEvent(SceneEvent* sceneEvent);
        
        UniquePtr<ECSManager> m_ECSManager;
        RefWrap<Game> m_Game;
        InputManager m_InputManager;
        UniquePtr<SceneEvent> m_SceneEvent;
        
    };
}
