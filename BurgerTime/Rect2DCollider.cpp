#include "Rect2DCollider.h"
#include "ColliderResolver.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Core/GameObject/GameObject.h"

Rect2DCollider::Rect2DCollider(powe::WorldEntity&, powe::GameObjectID ,
	powe::GameObjectID colliderManagerObject, const glm::fvec2& size, uint32_t layer, uint32_t targetLayer)
	: ColliderManager(colliderManagerObject)
	//, Owner(owner)
	, Size(size)
	, OnLayer(layer)
	, TargetLayer(targetLayer)
{

}

Rect2DCollider::Rect2DCollider(const SharedPtr<powe::GameObject>& ,
	const SharedPtr<powe::GameObject>& colliderManagerObject, const glm::fvec2& size, uint32_t layer,
	uint32_t targetLayer)
	//: Owner(owner->GetID())
	: ColliderManager(colliderManagerObject->GetID())
	, Size(size)
	, OnLayer(layer)
	, TargetLayer(targetLayer)
{
	colliderManagerObject;

}

void Rect2DCollider::OnCreate(powe::WorldEntity& worldEntity, powe::GameObjectID gameObjectId)
{
	ColliderResolver* colliderResolver = worldEntity.GetComponent<ColliderResolver>(ColliderManager);
	if (colliderResolver)
	{
		colliderResolver->AddCollider(gameObjectId, OnLayer);
	}
}
