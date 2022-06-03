#pragma once

#include <poweCustomtype.h>


namespace powe
{
	class WorldEntity;
}

class ColliderCommand
{
public:

	ColliderCommand() = default;
	virtual void OnEnter(powe::WorldEntity&, powe::GameObjectID owner, powe::GameObjectID other) = 0;
	virtual ~ColliderCommand() = default;
};

class DebugTriggerEnter : public ColliderCommand
{
public:

	void OnEnter(powe::WorldEntity&, powe::GameObjectID owner, powe::GameObjectID other) override;
};

class OnStaticIngredientTrigger : public ColliderCommand
{
public:

	void OnEnter(powe::WorldEntity&, powe::GameObjectID owner, powe::GameObjectID other) override;
};

