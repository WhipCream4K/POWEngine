#pragma once

#include <poweCustomtype.h>
#include <poweComponent.h>

#include "StaticVariables.h"
#include "ScorableEvent.h"

namespace powe
{
	class GameObject;
}

class SceneFactory;
class IngredientState;
class OnIngredientDropToPlatform;
class IngredientsComponent : public powe::Component<IngredientsComponent>
{
public:

	IngredientsComponent() = default;

	SharedPtr<IngredientState> CurrentState{};
	//SharedPtr<OnIngredientDropToPlatform> OnIngredientFinishedDrop{};
	SharedPtr<ScorableEvent> OnIngredientDropToPlatform{};
	WeakPtr<SceneFactory> SceneRef;
	std::vector<WeakPtr<powe::GameObject>> StepColliders;
	WeakPtr<powe::GameObject> Owner{};
	IngredientsType Type{};

	int LastPlayerStepIndex{};

	glm::fvec2 BounceStartPos{};
	float BouncingSpeed{};
	float BouncingStartVelocity{};
	float BounceApexTime{};
	float BounceTimeCounter{};
	bool IsOnPlate{};
	bool IsBouncing{};

	float FallingSpeed{};
	//int FallPassEnemiesCount{};

	int LevelIdx{};

	int StartCol{};
	int StartRow{};

	int CurrentStepCount{};
	int MaxStepCount{};

	float StepPushDistance{};
	float StepPushDecreaseOffset{};
};

