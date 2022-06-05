#pragma once

#include <poweCustomtype.h>


namespace powe
{
	class WorldEntity;
}

class Rect2DCollider;
class ColliderCommand
{
public:

	ColliderCommand() = default;

	virtual void OnEnter(powe::WorldEntity&, 
		Rect2DCollider* ownerCollider,
		Rect2DCollider* otherCollider ,
		powe::GameObjectID owner, 
		powe::GameObjectID other) = 0;

	virtual ~ColliderCommand() = default;
};

class DebugTriggerEnter : public ColliderCommand
{
public:

	void OnEnter(powe::WorldEntity&, Rect2DCollider* ownerCollider, Rect2DCollider* otherCollider, powe::GameObjectID owner, powe::GameObjectID other) override;
	//void OnEnter(powe::WorldEntity&, powe::GameObjectID owner, powe::GameObjectID other) override;
};

class OnStaticIngredientTrigger : public ColliderCommand
{
public:

	void OnEnter(powe::WorldEntity&, Rect2DCollider* ownerCollider, Rect2DCollider* otherCollider, powe::GameObjectID owner, powe::GameObjectID other) override;
	//void OnEnter(powe::WorldEntity&, powe::GameObjectID owner, powe::GameObjectID other) override;

private:

	float m_DropDownDistance{7.0f};
	float m_DecreaseOffset{2.0f};
};

//class PlateTriggerEnter : public ColliderCommand
//{
//public:
//
//	void OnEnter(powe::WorldEntity&, Rect2DCollider* ownerCollider, Rect2DCollider* otherCollider, powe::GameObjectID owner, powe::GameObjectID other) override;
//	//void OnEnter(powe::WorldEntity&, powe::GameObjectID owner, powe::GameObjectID other) override;
//};

class OnFallingIngredientTrigger : public ColliderCommand
{
public:

	void OnEnter(powe::WorldEntity&, Rect2DCollider* ownerCollider, Rect2DCollider* otherCollider, powe::GameObjectID owner, powe::GameObjectID other) override;
};

