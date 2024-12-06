#pragma once

// #include "SceneQuery.h"
#include "Core/CustomTypes.h"
#include "ECS/ECSTypes.h"
#include "ECS/ComponentView.h"

namespace powe
{
    class Scene;

    template<typename F,typename ...Args>
    concept TaskConcept = std::is_invocable_v<F, ComponentView&>;

    class SceneSystem
    {

        using ComponentTask = std::pair<std::function<void(ComponentView&)>,ComponentView>;

    public:
        
        SceneSystem(Scene& sceneRef);
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

        ECSManager& GetECSManager() const noexcept;

        template<typename Func,typename Tuple = FuncInfo<Func>::arg_types>
        void ForEach(Func&& func)
        {

            std::function<void(ComponentView&)> task{[f = std::forward<Func>(func)](ComponentView& view){
                
                // the component type is std::tuple<Args&...>
                std::for_each(view.begin<Tuple>(),view.end<Tuple>(),[&f](auto& comp)
                {
                    // expand the tuple to arguments pack
                    ForEachTuple(
                        std::forward<Func>(f),std::forward<Tuple>(comp),
                    std::make_index_sequence<std::tuple_size_v<Tuple>>{});
                });

             }};

            // Create ComponentView for this
            auto& ecsManager{GetECSManager()};
            constexpr auto compIDs{MakeComponentVec<Tuple>()};
            const ComponentView view{ecsManager,compIDs};

            // TODO: Seperate Create, Start, Update, Exit task
            m_UpdateTasks.emplace_back(std::make_pair(task,view));
        }

        template<typename T>
        requires std::is_base_of_v<SceneSystem, T>
        void RunAfter()
        {
            // TODO: Finish this function
        }

    private:

        template<typename Func,typename Tuple,std::size_t... Is>
        static void ForEachTuple(Func&& func, Tuple&& tuple, std::index_sequence<Is...>)
        {
            func(std::get<Is>(tuple)...);
        }

        RefWrap<Scene> m_Scene;

        Vector<ComponentTask> m_CreateTasks;
        Vector<ComponentTask> m_StartTasks;
        Vector<ComponentTask> m_UpdateTasks;
        Vector<ComponentTask> m_ExitTasks;
        
    };

    template <typename T>
    concept CSceneSystem = std::is_base_of_v<SceneSystem, T>;
}
