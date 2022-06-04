#pragma once

#include <poweCustomtype.h>
#include <poweComponent.h>

#include "StaticVariables.h"

namespace powe
{
	class GameObject;
}

class SceneFactory;
class IngredientState;
class IngredientsComponent : public powe::Component<IngredientsComponent>
{
public:

	IngredientsComponent() = default;

	SharedPtr<IngredientState> CurrentState{};
	WeakPtr<SceneFactory> SceneRef;
	std::vector<WeakPtr<powe::GameObject>> StepColliders;
	WeakPtr<powe::GameObject> Owner{};
	IngredientsType Type{};

	glm::fvec2 BounceStartPos{};
	float BouncingSpeed{};
	float BouncingAccel{};
	float BounceApexTime{};
	float BounceTimeCounter{};
	bool IsBouncing{};

	float FallingSpeed{};

	int LevelIdx{};

	int StartCol{};
	int StartRow{};

	int CurrentStepCount{};
	int MaxStepCount{};

	float StepPushDistance{};
	float StepPushDecreaseOffset{};
};

