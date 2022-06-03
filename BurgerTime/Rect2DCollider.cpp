#include "Rect2DCollider.h"
#include "ColliderResolver.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Core/GameObject/GameObject.h"

Rect2DCollider::Rect2DCollider(powe::WorldEntity& worldEntity, powe::GameObjectID owner,
	powe::GameObjectID colliderManagerObject, const glm::fvec2& size, uint32_t layer, uint32_t targetLayer)
	: ColliderManager(colliderManagerObject)
	, Size(size)
	, OnLayer(layer)
	, TargetLayer(targetLayer)
{
	if (owner)
	{
		ColliderResolver* colliderResolver = worldEntity.GetComponent<ColliderResolver>(colliderManagerObject);
		if (colliderResolver)
		{
			colliderResolver->AddCollider(owner, layer);
		}
	}
}

Rect2DCollider::Rect2DCollider(const SharedPtr<powe::GameObject>& owner,
							   const SharedPtr<powe::GameObject>& colliderManagerObject, const glm::fvec2& size, uint32_t layer,
							   uint32_t targetLayer)
	: Size(size)
	, OnLayer(layer)
	, TargetLayer(targetLayer)
{
	if (owner)
	{
		ColliderResolver* colliderResolver = colliderManagerObject->GetComponent<ColliderResolver>();
		if (colliderResolver)
		{
			ColliderManager = colliderManagerObject->GetID();
			colliderResolver->AddCollider(owner->GetID(), layer);
		}
	}
}



void Rect2DCollider::OnDestroy(powe::WorldEntity& worldEntity, powe::GameObjectID gameObjectId)
{
	if(ColliderResolver* colliderResolver = worldEntity.GetComponent<ColliderResolver>(ColliderManager))
	{
		colliderResolver->RemoveCollider(gameObjectId);
	}
}
