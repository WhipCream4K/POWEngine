#pragma once
#include "UserComponents.h"
#include "POWEngine/ECS/SystemBase.h"


namespace powe
{
    class ThreadExecute;
    class GameObject;
}

class WanderAsync;
class AsyncSystemTest :
    public powe::SystemBase,
    public std::enable_shared_from_this<AsyncSystemTest>
{
public:
    
    AsyncSystemTest(const SharedPtr<powe::GameObject>& sceneObject);

protected:
    
    void OnUpdate(float, powe::GameObjectID) override
    {
    }
    void OnPreCreate() override;
    void InternalUpdate(powe::Archetype*, float) override;
    void OnDestroy(powe::GameObjectID) override;

private:
    
    void ExecuteTasks(powe::Archetype* archetype,float deltaTime);

    std::vector<SharedPtr<WanderAsync>> m_ThreadTasks{};
    std::future<void> m_TaskFuture{};
    WeakPtr<powe::GameObject> m_SceneObject{};
    SceneComponent* m_SceneComponent{};
    DebugSteeringComponent* m_DebugSteering{};
};
