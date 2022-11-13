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

AsyncSystemTest::AsyncSystemTest()
{
    DefineSystemKeys<Transform2D, VelocityComponent>();
}

void AsyncSystemTest::OnPreCreate()
{
    if(!m_SceneComponent)
    {
        m_SceneComponent = m_SceneObject.lock()->GetComponent<SceneComponent>();

        for (int i = 0; i < m_SceneComponent->asyncObjects.size(); ++i)
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
    }
    else
    {
        // block
        for (const auto& threadTask : m_ThreadTasks)
        {
            if(!threadTask->IsFinished())
                return;
        }

        // start again
        m_TaskFuture = world.GetThreadPool()->PushMemberTask(
            &AsyncSystemTest::ExecuteTasks,
            shared_from_this(),
            archetype,
            deltaTime);
    }
}

void AsyncSystemTest::OnDestroy(powe::GameObjectID)
{
    m_TaskFuture.get();
}

void AsyncSystemTest::ExecuteTasks(Archetype* archetype, float deltaTime)
{
    // might blow up
    auto* world{&GetWorld()};

    if (!m_SceneObject.lock())
        return;

    // might blow up
    
    for (SizeType i = 0; i < archetype->GameObjectIds.size(); ++i)
    {
        const auto& taskExecute{m_ThreadTasks[i]};
        taskExecute->OnCreate(world, archetype, deltaTime);
        const auto& renderObject{m_SceneComponent->asyncObjects[i]};

        // might blow up
        AsyncRender* async_render{renderObject->GetComponent<AsyncRender>()};
        if(!async_render)
            continue;
    
        {
            std::scoped_lock updateLock{async_render->taskLock };
            
            async_render->transformUpdate = world->GetThreadPool()->PushMemberTask(
                &WanderAsync::CalculatePosition,
                taskExecute,
                deltaTime,
                archetype->GameObjectIds[i]);
        }
        
    }
}
