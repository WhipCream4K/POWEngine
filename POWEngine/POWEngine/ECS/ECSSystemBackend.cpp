#include "pch.h"
#include "ECSSystemBackend.h"

void powe::ECSSystemBackend::Initialize(WorldEntity& worldEntity)
{
    m_World = &worldEntity;
    OnCreate();
}
