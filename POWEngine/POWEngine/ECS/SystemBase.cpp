#include "pch.h"
#include "SystemBase.h"

#include "POWEngine/ECS/Archetype.h"
#include "POWEngine/Logger/LoggerUtils.h"

powe::SystemBase::SystemBase()
	: m_World()
	, m_Keys()
	, m_CurrentArchetype()
	, m_UpdateCountPerArchetype()
{
}

void powe::SystemBase::InternalUpdate(const Archetype& archetype, float deltaTime)
{
	m_UpdateCountPerArchetype = 0;
	m_CurrentArchetype = &archetype;

	try
	{
		for (const auto gameObjectId : archetype.GameObjectIds)
		{
			OnUpdate(deltaTime, gameObjectId);
			++m_UpdateCountPerArchetype;
		}
	}
	catch (const std::exception& e)
	{
		std::string errMsg{ "System trying to access component that doesn't exist in this archetype -> " };
		errMsg.append(e.what());
		POWLOGWARNING(errMsg);

		m_UpdateCountPerArchetype = 0;
		m_CurrentArchetype = nullptr;
	}

}

void powe::SystemBase::InternalCreate(const Archetype& archetype)
{
	m_UpdateCountPerArchetype = 0;
	m_CurrentArchetype = &archetype;

	try
	{
		for (const auto gameObjectId : archetype.GameObjectIds)
		{
			OnCreate(gameObjectId);
			++m_UpdateCountPerArchetype;
		}
	}
	catch (const std::exception& e)
	{
		std::string errMsg{ "System trying to access component that doesn't exist in this archetype -> " };
		errMsg.append(e.what());
		POWLOGWARNING(errMsg);

		m_UpdateCountPerArchetype = 0;
		m_CurrentArchetype = nullptr;
	}
}

void powe::SystemBase::InternalDestroy(const Archetype& archetype)
{
	m_UpdateCountPerArchetype = 0;
	m_CurrentArchetype = &archetype;

	try
	{
		for (const auto gameObjectId : archetype.GameObjectIds)
		{
			OnDestroy(gameObjectId);
			++m_UpdateCountPerArchetype;
		}
	}
	catch (const std::exception& e)
	{
		std::string errMsg{ "System trying to access component that doesn't exist in this archetype -> " };
		errMsg.append(e.what());
		POWLOGWARNING(errMsg);

		m_UpdateCountPerArchetype = 0;
		m_CurrentArchetype = nullptr;
	}
}

void powe::SystemBase::SetWorld(WorldEntity* world)
{
	m_World = world;
}

