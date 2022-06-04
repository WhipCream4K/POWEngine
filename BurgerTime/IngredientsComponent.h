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
	IngredientsType Type{};
	int StartCol{};
	int StartRow{};
	int CurrentStepCount{};
	int MaxStepCount{};
};

