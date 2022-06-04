#pragma once

#include <poweCustomtype.h>

class IngredientsComponent;

namespace powe
{
	class WorldEntity;
}

class IngredientState 
{
public:

	IngredientState() = default;
	virtual ~IngredientState() = default;

	static SharedPtr<IngredientState> Stationary;
	static SharedPtr<IngredientState> Falling;
	static SharedPtr<IngredientState> Spawn;
	static SharedPtr<IngredientState> Bounce;

	virtual SharedPtr<IngredientState> Update(powe::WorldEntity&, float,IngredientsComponent*,powe::GameObjectID) { return {}; }
	virtual void Enter(powe::WorldEntity&, IngredientsComponent*, powe::GameObjectID) {}
	virtual void Exit(powe::WorldEntity&, IngredientsComponent*, powe::GameObjectID) {}
};

class SpawnIngredient : public IngredientState
{
public:

	SharedPtr<IngredientState> Update(powe::WorldEntity&, float, IngredientsComponent*, powe::GameObjectID) override;

};

class StationaryIngredient : public IngredientState
{
public:

	SharedPtr<IngredientState> Update(powe::WorldEntity&, float, IngredientsComponent*, powe::GameObjectID) override;
	void Enter(powe::WorldEntity&, IngredientsComponent*, powe::GameObjectID) override;
	void Exit(powe::WorldEntity&, IngredientsComponent*, powe::GameObjectID) override;
};

class FallingIngredient : public IngredientState
{

public:

	SharedPtr<IngredientState> Update(powe::WorldEntity&, float, IngredientsComponent*, powe::GameObjectID) override;
	void Enter(powe::WorldEntity&, IngredientsComponent*, powe::GameObjectID) override;
	void Exit(powe::WorldEntity&, IngredientsComponent*, powe::GameObjectID) override;
};

class Bouncing : public IngredientState
{
public:

	SharedPtr<IngredientState> Update(powe::WorldEntity&, float, IngredientsComponent*, powe::GameObjectID) override;
	void Enter(powe::WorldEntity&, IngredientsComponent*, powe::GameObjectID) override;
	void Exit(powe::WorldEntity&, IngredientsComponent*, powe::GameObjectID) override;
};


