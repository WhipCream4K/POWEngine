#include "pch.h"
#include "SystemBase.h"

powe::SystemBase::SystemBase(const SharedPtr<WorldEntity>& world, PipelineLayer layer)
	: m_Keys()
	, m_World(world)
	, m_Layer(layer)
	, m_MarkedDeleted()
{
}

