#include "ColliderCommand.h"

#include "POWEngine/Logger/LoggerUtils.h"
#include "BurgerTimeComponents.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

void DebugTriggerEnter::OnEnter(powe::WorldEntity&, powe::GameObjectID owner, powe::GameObjectID other)
{
	std::stringstream enter{"This "};
	enter << owner << " Hit : " << other;
	POWLOGNORMAL(enter.str());
}

void OnStaticIngredientTrigger::OnEnter(powe::WorldEntity& worldEntity, powe::GameObjectID owner, powe::GameObjectID other)
{
	// If is player
	if(worldEntity.GetComponent<PlayerTag>(other))
	{
		std::stringstream enter{ "This " };
		enter << owner << " Hit : " << other;
		POWLOGNORMAL(enter.str());

		worldEntity.AddComponentToGameObject(owner, FallingComponent{80.0f});
	}
}
