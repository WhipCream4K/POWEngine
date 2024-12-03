#pragma once

// #include "SceneQuery.h"
#include "Core/CustomTypes.h"
#include "ECS/ECSTypes.h"
#include "ECS/ComponentView.h"

namespace powe
{
    class Scene;
    class IArchetype;

    template<typename F,typename ...Args>
    concept TaskConcept = std::is_invocable_v<F, ComponentView&>;

    class SceneSystem
    {

        using ComponentTask = std::pair<std::function<void(ComponentView&)>,ComponentView>;

    public:
        
        SceneSystem() = default;
        SceneSystem(const SceneSystem&) = delete;
        SceneSystem& operator=(const SceneSystem&) = delete;
        SceneSystem(SceneSystem&&) = delete;
        SceneSystem& operator=(SceneSystem&&) = delete;
        virtual ~SceneSystem() = default;

        virtual void OnCreate(Scene&) {}
        virtual void OnStart(Scene&) {}
        virtual void OnUpdate(Scene& scene, float) = 0;
        virtual void OnExit(Scene&) = 0;

    protected:

        template<typename Func,typename ArgsTypes = FuncInfo<Func>::arg_types>
        // requires ComponentConcept<std::tuple_element_t<0,ArgsTypes>>
        void ForEach(Func&& func)
        {
            using FI = FuncInfo<Func>;
            using ArgumentTypes = FI::arg_types;

            std::function<void(ComponentView&)> task{[f = std::forward<Func>(func)](ComponentView& view){
                view.Visit(f);
             }};

            // Create ComponentView for this


            // m_UpdateTasks.emplace_back(task);
        }

        template<typename T>
        requires std::is_base_of_v<SceneSystem, T>
        void RunAfter()
        {
            // TODO: Finish this function
        }

    private:

        Vector<ComponentTask> m_CreateTasks;
        Vector<ComponentTask> m_StartTasks;
        Vector<ComponentTask> m_UpdateTasks;
        Vector<ComponentTask> m_ExitTasks;
        
    };

    template <typename T>
    concept CSceneSystem = std::is_base_of_v<SceneSystem, T>;
}
