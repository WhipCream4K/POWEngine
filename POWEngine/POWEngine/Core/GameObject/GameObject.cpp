#include "pch.h"
#include "GameObject.h"

#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Logger/LoggingService.h"

//powe::GameObject::GameObject(const SharedPtr<WorldEntity>& world)
//	: m_World(world)
//	, m_Id()
//{
//	if(!world)
//	{
//		//POWLOGERROR("GameObject cannot exist without world entity");
//		throw std::exception("GameObject instance cannot exist without world entity");
//	}
//
//	m_Id = world->GetNewEntityID();
//	world->RegisterGameObject(m_Id);
//}

powe::GameObject::GameObject(WorldEntity& world)
	: m_World(world)
	, m_Id(world.GetNewEntityID())
{
	world.RegisterGameObject(m_Id);
}

//void powe::GameObject::SetParent(const SharedPtr<GameObject>& )
//{
//	//if(gameObject)
//	//{
//	//	if(const auto parent{m_ParentNode.lock()})
//	//	{
//	//		parent->SetParent(nullptr);
//	//	}
//	//	else
//	//	{
//	//		
//	//	}
//	//}
//	//else
//	//{
//	//	
//	//}
//}
