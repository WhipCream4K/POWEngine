#include "pch.h"
#include "Entity.h"

#include "ECSManager.h"

using namespace powe;

Entity::Entity(ECSManager& manager) noexcept
    : m_ECS(manager)
{
    m_ID = manager.MakeNewEntityID();
}

Entity::~Entity()
{
    m_ECS->Remove(m_ID);
}