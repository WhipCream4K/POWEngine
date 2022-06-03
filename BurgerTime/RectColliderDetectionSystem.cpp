#include "RectColliderDetectionSystem.h"

#include "ColliderResolver.h"
#include "Rect2DCollider.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "ColliderCommand.h"

RectColliderDetectionSystem::RectColliderDetectionSystem()
{
	DEFINE_SYSTEM_KEY(ColliderResolver);
}

void RectColliderDetectionSystem::OnUpdate(float, powe::GameObjectID)
{
	using namespace powe;
	ColliderResolver* colliderManager{ GetComponent<ColliderResolver>() };

	WorldEntity* worldEntity{ GetWorld() };

	for (const GameObjectID thisCollider : colliderManager->GetAllColliders())
	{
		Rect2DCollider* rect2D{ worldEntity->GetComponent<Rect2DCollider>(thisCollider) };
		if (rect2D)
		{
			if(!rect2D->OnEnterCallback)
				continue;

			for (uint32_t i = 0; i < OverlapLayer::Count; ++i)
			{
				const uint32_t layer{ 1u << i };

				if (layer & rect2D->TargetLayer)
				{
					std::ranges::for_each(colliderManager->GetColliderLayer(i), [&thisCollider, worldEntity, &rect2D](const GameObjectID other)
						{
							if (thisCollider != other)
							{
								// DO simple rectangle overlap detection
								Transform2D* thisColliderTransform{ worldEntity->GetComponent<Transform2D>(thisCollider) };
								Transform2D* otherColliderTransform{ worldEntity->GetComponent<Transform2D>(other) };
								const Rect2DCollider* otherColliderComp{ worldEntity->GetComponent<Rect2DCollider>(other) };

								const auto& thisColliderPos{ thisColliderTransform->GetWorldPosition() };
								const auto& thisColliderExtent{ rect2D->Size };
								const auto& otherColliderPos{ otherColliderTransform->GetWorldPosition() };
								const auto& otherColliderExtent{ otherColliderComp->Size };

								if(!rect2D->IsEntering)
								{
									if (IsOverlap(thisColliderPos, thisColliderExtent, otherColliderPos, otherColliderExtent))
									{
										rect2D->IsEntering = true;
										if (rect2D->OnEnterCallback)
											rect2D->OnEnterCallback->OnEnter(*worldEntity, thisCollider, other);

										if (otherColliderComp->OnEnterCallback)
											otherColliderComp->OnEnterCallback->OnEnter(*worldEntity, other, thisCollider);
									}
								}
								else
								{
									if (!IsOverlap(thisColliderPos, thisColliderExtent, otherColliderPos, otherColliderExtent))
										rect2D->IsEntering = false;
								}

							}
						});
				}
			}


		}
	}
}

bool RectColliderDetectionSystem::IsOverlap(const glm::fvec2& pos1, const glm::fvec2& extent1, const glm::fvec2& pos2,
	const glm::fvec2& extent2)
{
	// https://silentmatt.com/rectangle-intersection/
	// for visualisation
	// although this version has to adapt to the current coordinates
	const bool left{ pos1.x - extent1.x < pos2.x + extent2.x };
	const bool right{ pos1.x + extent1.x > pos2.x - extent2.x };
	const bool top{ pos1.y - extent1.y < pos2.y + extent2.y };
	const bool bottom{ pos1.y + extent1.y > pos2.y - extent2.y };

	return left && right && top && bottom;
}