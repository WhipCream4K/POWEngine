#include "FallingSystem.h"

#include "BurgerTimeComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"

FallingSystem::FallingSystem()
{
	using namespace powe;
	DEFINE_SYSTEM_KEY(Transform2D,FallingComponent);
}

void FallingSystem::OnUpdate(float delta, powe::GameObjectID)
{
	using namespace powe;

	const auto& [transform, falling] = GetComponentsView<Transform2D, FallingComponent>();

	glm::fvec2 oldPos{ transform->GetWorldPosition() };
	oldPos.y += falling->speed * delta;

	transform->SetWorldPosition(oldPos);
}
