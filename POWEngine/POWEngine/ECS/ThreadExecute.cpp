#include "pch.h"
#include "ThreadExecute.h"

#include "POWEngine/Logger/LoggerUtils.h"


void powe::ThreadExecute::OnCreate(WorldEntity* world, Archetype* archetype, float)
{
    m_CurrentArchetype = archetype;
    m_World = world;
    m_TaskFinished = false;
}

void powe::ThreadExecute::SetTaskState(bool state)
{
    m_TaskFinished = state;
}

bool powe::ThreadExecute::IsFinished() const
{
    return m_TaskFinished;
}
