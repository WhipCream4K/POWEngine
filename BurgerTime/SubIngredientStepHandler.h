#pragma once

#include <poweComponent.h>

namespace powe
{
	class GameObject;
}
class SceneFactory;
class SubIngredientStepHandler : public powe::Component<SubIngredientStepHandler>
{
public:

	SubIngredientStepHandler() = default;
	SubIngredientStepHandler(const SharedPtr<powe::GameObject>& owner,int maxStepCount);
	void IncreaseStep();

private:

	//powe::GameObjectID m_Owner;
	WeakPtr<powe::GameObject> m_Owner;
	WeakPtr<SceneFactory> m_Scene;
	int m_StepCount{};
	int m_MaxStepCount{};
};

