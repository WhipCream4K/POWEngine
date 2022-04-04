#pragma once

#include "POWEngine/Core/ECS/ECSTypes.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
//#include "POWEngine/Core/GameObject/GameObject.h"

namespace powe
{
	template<typename T>
	class Attachable
	{
		friend class WorldEntity;

	public:

		T* GetParent();
		void SetParent(GameObjectID id);
		virtual void Update(T* parent) = 0;

	public:

		Attachable(const Attachable&) = default;
		Attachable& operator=(const Attachable&) = delete;
		Attachable(Attachable&&) = default;
		Attachable& operator=(Attachable&&) = delete;
		virtual ~Attachable() = default;

	protected:

		explicit Attachable(WorldEntity& world);

		WorldEntity& m_World;
		GameObjectID m_ParentID;
		//WeakPtr<GameObject> m_ParentNode;
	};

	template <typename T>
	T* Attachable<T>::GetParent()
	{
		if (m_ParentID != INVALID_GAMEOBJECT_ID)
		{
			// During GetParent maybe the object is already destroy so we need to update that in run-time
			if (T * parentComponent{ m_World.GetComponentByGameObject<T>(m_ParentID) })
				return parentComponent;

			m_ParentID = INVALID_GAMEOBJECT_ID;
			Update(nullptr);
		}

		return nullptr;
	}

	template <typename T>
	void Attachable<T>::SetParent(GameObjectID id)
	{
		if (id == INVALID_GAMEOBJECT_ID)
		{
			m_ParentID = INVALID_GAMEOBJECT_ID;
			Update(nullptr);
		}
		else
		{
			m_ParentID = id;
			Update(GetParent());
		}
	}

	template <typename T>
	Attachable<T>::Attachable(WorldEntity& world)
		: m_World(world)
		, m_ParentID()
	{
	}
}


