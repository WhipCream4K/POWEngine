#pragma once

#include <poweComponent.h>
#include "POWEngine/Math/Math.h"
#include "StaticVariables.h"

namespace powe
{
	class GameObject;
}

class ColliderCommand;
class Rect2DCollider : public powe::Component<Rect2DCollider>
{
public:

	Rect2DCollider() = default;

	Rect2DCollider(
		powe::WorldEntity& worldEntity,
		powe::GameObjectID owner,
		powe::GameObjectID colliderManagerObject,
		const glm::fvec2& size,
		uint32_t layer,uint32_t targetLayer = 0);

	Rect2DCollider(
		const SharedPtr<powe::GameObject>& owner,
		const SharedPtr<powe::GameObject>& colliderManagerObject,
		const glm::fvec2& size,
		uint32_t layer, uint32_t targetLayer = 0);

	void OnDestroy(powe::WorldEntity&, powe::GameObjectID) override;

	SharedPtr<ColliderCommand> OnEnterCallback{};
	powe::GameObjectID ColliderManager{};
	glm::fvec2 Size{};
	uint32_t OnLayer{};
	uint32_t TargetLayer{};
	bool IsEntering{};
};

