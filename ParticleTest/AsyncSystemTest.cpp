#include "AsyncSystemTest.h"

#include <iostream>

#include "UserComponents.h"
#define RELEASE_WITH_LOGINFO
#include "WanderAsync.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/ECS/ThreadExecute.h"
#include "POWEngine/Logger/LoggerUtils.h"
using namespace powe;

AsyncSystemTest::AsyncSystemTest(const SharedPtr<powe::GameObject>& sceneObject)
    : m_SceneObject(sceneObject)
{
    DefineSystemKeys<PositionComponent, VelocityComponent>();
}

void AsyncSystemTest::OnPreCreate()
{
    if (!m_SceneComponent)
    {
        m_SceneComponent = m_SceneObject.lock()->GetComponent<SceneComponent>();
        m_DebugSteering = m_SceneObject.lock()->GetComponent<DebugSteeringComponent>();

        for (SizeType i = 0; i < m_SceneComponent->drawObjects.size(); ++i)
        {
            m_ThreadTasks.emplace_back(std::make_shared<WanderAsync>());
        }
    }
}

void AsyncSystemTest::InternalUpdate(powe::Archetype* archetype, float deltaTime)
{
    const auto& world{GetWorld()};

    if (!m_TaskFuture.valid())
    {
        m_TaskFuture = world.GetThreadPool()->PushMemberTask(
            &AsyncSystemTest::ExecuteTasks,
            shared_from_this(),
            archetype,
            deltaTime);

        return;
    }

    // block
    for (SizeType i = 0; i < archetype->GameObjectIds.size(); ++i)
    {
        const auto& threadTask{m_ThreadTasks[i]};
        if(!threadTask->IsFinished())
            return;
    }
    
    // m_TaskFuture.wait();

    // start again
    m_TaskFuture = world.GetThreadPool()->PushMemberTask(
        &AsyncSystemTest::ExecuteTasks,
        shared_from_this(),
        archetype,
        deltaTime);
    
}

void AsyncSystemTest::OnDestroy(powe::GameObjectID)
{
    m_TaskFuture.get();
}

void AsyncSystemTest::ExecuteTasks(Archetype* archetype [[maybe_unused]], float deltaTime [[maybe_unused]])
{
    // POWLOGWARNING("This Thread finished");
    // might blow up
    auto* world{&GetWorld()};

    if (!m_SceneComponent && !m_DebugSteering)
        return;

    // might blow up

    for (SizeType i = 0; i < archetype->GameObjectIds.size(); ++i)
    {
        const auto& taskExecute{m_ThreadTasks[i]};
        taskExecute->OnCreate(world, archetype,i);
        const auto& renderObject{m_SceneComponent->drawObjects[i]};

        // might blow up
        AsyncRender* async_render{renderObject->GetComponent<AsyncRender>()};
        if (!async_render)
            continue;

        {
            std::scoped_lock updateLock{async_render->taskLock};

            async_render->transformUpdate = world->GetThreadPool()->PushMemberTask(
                &WanderAsync::CalculatePosition,
                taskExecute,
                m_DebugSteering->boundArea,
                deltaTime,
                archetype->GameObjectIds[i]);
        }
    }
}
