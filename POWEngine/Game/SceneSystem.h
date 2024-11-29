#pragma once

namespace powe
{
    class Scene;
    class IArchetype;

    class SceneSystem
    {
    public:
        
        SceneSystem() = default;
        SceneSystem(const SceneSystem&) = delete;
        SceneSystem& operator=(const SceneSystem&) = delete;
        SceneSystem(SceneSystem&&) = delete;
        SceneSystem& operator=(SceneSystem&&) = delete;
        virtual ~SceneSystem() = default;

        virtual void OnInitialize() = 0;
        virtual void OnStart(Scene&) = 0;
        virtual void OnUpdate(Scene& scene, float) = 0;
        virtual void OnExit(Scene&) = 0;

    private:
    
        
    };

    template <typename T>
    concept CSceneSystem = std::is_base_of_v<SceneSystem, T>;
}
