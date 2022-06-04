#pragma once

#include <poweComponent.h>

class SubIngredientComponent : public powe::Component<SubIngredientComponent>
{
public:

	SubIngredientComponent() = default;
	SubIngredientComponent(powe::GameObjectID stepCounterObject);

	void TriggerStepOn();
	
private:

	powe::GameObjectID m_StepCounterObject;
};

