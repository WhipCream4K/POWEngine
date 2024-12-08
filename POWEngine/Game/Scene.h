#pragma once

#include <future>


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

    class SimpleThreadPool;
    class Scene final
    {
    public:
        
        Scene(Game& game);
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene(Scene&&) = default;
        Scene& operator=(Scene&&) = default;
        ~Scene();

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
        std::future<void> m_SysytemFuture;

        UniquePtr<ECSManager> m_ECSManager;
        WeakPtr<SimpleThreadPool> m_ThreadPoolModule;
        RefWrap<Game> m_Game;
        
    };
}
