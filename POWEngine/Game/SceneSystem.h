#pragma once

#include "ECS/ECSManager.h"
#include "Core/CustomTypes.h"

namespace powe
{
    class Scene;

    class SceneSystem
    {
    public:
        
        SceneSystem(Scene& sceneRef);
        SceneSystem(const SceneSystem&) = delete;
        SceneSystem& operator=(const SceneSystem&) = delete;
        SceneSystem(SceneSystem&&) = delete;
        SceneSystem& operator=(SceneSystem&&) = delete;
        virtual ~SceneSystem() = default;

        virtual void OnCreate(Scene&) {};
        virtual void OnStart(Scene&) {}
        virtual void OnUpdate(Scene& scene, float) = 0;
        virtual void OnExit(Scene&) = 0;

    protected:


        ECSManager& GetECSManager() const noexcept;

        template<typename T>
        requires std::is_base_of_v<SceneSystem, T>
        void RunAfter()
        {
            // TODO: Finish this function
        }

    private:

        RefWrap<Scene> m_Scene;
    };

    template <typename T>
    concept CSceneSystem = std::is_base_of_v<SceneSystem, T>;
}
