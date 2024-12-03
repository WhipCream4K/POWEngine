#include "pch.h"
#include "Entity.h"

#include "ECSManager.h"

using namespace powe;

Entity::Entity(ECSManager& manager)
    : m_ECS(manager)
{
    m_ID = manager.MakeNewEntityID();
}