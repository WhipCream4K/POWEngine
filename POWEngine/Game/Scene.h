#pragma once

#include "Utils/Utils.h"
#include "Game/Input/InputManager.h"

namespace powe
{
    class ECSManager;
    class EngineLayer;
    class Game;
    class SceneEvent;
    class SceneSystem;

    enum class SchedulePolicy : uint8_t
    {
        Sequence,
        Unsequence,
        Detach
    };

    class Scene final
    {
    public:
        
        Scene(Game& game);
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene(Scene&&) = default;
        Scene& operator=(Scene&&) = default;
        ~Scene() = default;

        void Start();
        void Exit();
        void Update(float deltaTime);

        void SceduleSystem(const SharedPtr<SceneSystem>& system, 
        SchedulePolicy policy = SchedulePolicy::Sequence) noexcept;
        

        ECSManager& GetECSManager() const { return *m_ECSManager.get(); }
        Game& GetGameModule() const noexcept { return m_Game; }
        std::string_view GetName() const noexcept;
        SharedPtr<PMRResource> GetResource() const noexcept;

    private:

        
        Vector<SharedPtr<SceneSystem>> m_SequenceSystems;
        Vector<SharedPtr<SceneSystem>> m_UnsequenceSystems;
        Vector<SharedPtr<SceneSystem>> m_DetachSystem;

        UniquePtr<ECSManager> m_ECSManager;
        RefWrap<Game> m_Game;
        // InputManager m_InputManager;

        // TODO: Really can't use this scene event because we need run-time update manipulation
        // UniquePtr<SceneEvent> m_SceneEvent;
        
    };
}
