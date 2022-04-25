#include "pch.h"
#include "SystemBase.h"

#include "POWEngine/Core/WorldEntity/WorldEntity.h"

//powe::SystemBase::SystemBase(const SharedPtr<WorldEntity>& world, PipelineLayer layer)
//	: m_Keys()
//	, m_World(world)
//	, m_Layer(layer)
//	, m_MarkedDeleted()
//{
//	if (!world)
//		throw std::exception("system requires world to be valid");
//}

//powe::SystemBase::SystemBase(WorldEntity& world, PipelineLayer layer)
//	: m_Keys()
//	, m_World(world)
//	, m_Layer(layer)
//	, m_MarkedDeleted()
//{
//}

//powe::SystemBase::SystemBase(WorldEntity& world, PipelineLayer layer,
//	const std::vector<ComponentTypeID>& )
//	: m_World(world)
//	, m_Keys()
//	, m_Layer(layer)
//	, m_MarkedDeleted()
//{
//
//}

powe::SystemBase::SystemBase(WorldEntity& world, PipelineLayer layer)
	: m_World(world)
	, m_Layer(layer)
	, m_Keys()
	, m_UpdateCountPerArchetype()
{
	m_Keys = GET_SYSKEY();
}

void powe::SystemBase::InternalUpdate(const Archetype& archetype, float deltaTime)
{
	m_UpdateCountPerArchetype = 0;

	for (const auto& gID : archetype.GameObjectIds)
	{
		OnUpdate(deltaTime, gID);
		++m_UpdateCountPerArchetype;
	}
}
