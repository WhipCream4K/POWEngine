#include "pch.h"
#include "ThreadExecute.h"

#include "POWEngine/Logger/LoggerUtils.h"


void powe::ThreadExecute::OnCreate(WorldEntity* world, const Archetype* archetype,uint32_t id)
{
    m_CurrentArchetype = archetype;
    m_World = world;
    m_TaskFinished = false;
    m_UpdateIndex = id;
}

void powe::ThreadExecute::SetTaskState(bool state)
{
    m_TaskFinished = state;
}

bool powe::ThreadExecute::IsFinished() const
{
    return m_TaskFinished;
}
