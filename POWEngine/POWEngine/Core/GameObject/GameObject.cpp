#include "pch.h"
#include "GameObject.h"

#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Logger/LoggingService.h"

//SharedPtr<powe::GameObject> powe::GameObject::Create(const SharedPtr<WorldEntity>& world)
//{
//	SharedPtr<GameObject> gob{ std::make_shared<GameObject>(world) };
//	gob->m_Id = world->GetEntityId();
//	return gob;
//}

//powe::GameObject::GameObject(const SharedPtr<WorldEntity>& world)
//	: m_World(world)
//	, m_Id()
//{
//	if (world)
//	{
//		m_Id = world->GetEntityId();
//	}
//}

//powe::GameObject::GameObject(WorldEntity& world)
//	: m_World(world)
//	, m_Id(m_World.GetEntityId())
//{
//}

//SharedPtr<powe::GameObject> powe::Create(const SharedPtr<WorldEntity>& world)
//{
//	if (!world)
//		POWLOGERROR("GameObject cannot exist without world entity");
//
//	SharedPtr<GameObject> gob{ std::make_shared<GameObject>(world) };
//	gob->m_Id = world->GetEntityId();
//	return gob;
//}

powe::GameObject::GameObject(const SharedPtr<WorldEntity>& world)
	: m_World(world)
	, m_Id()
{
	if(!world)
	{
		//POWLOGERROR("GameObject cannot exist without world entity");
		throw std::exception("GameObject instance cannot exist without world entity");
	}

	m_Id = world->GetNewEntityID();
	world->RegisterGameObject(m_Id);
}

void powe::GameObject::SetParent(const SharedPtr<GameObject>& )
{

}
