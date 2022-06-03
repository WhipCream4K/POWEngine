#pragma once

#include <poweCustomtype.h>
#include <poweComponent.h>

#include "StaticVariables.h"

//struct TriggerPair
//{
//	powe::GameObjectID entityA{};
//	powe::GameObjectID entityB{};
//};

class ColliderResolver : public powe::Component<ColliderResolver>
{
public:

	ColliderResolver() = default;

	//std::vector<powe::GameObjectID> Colliders; // O(n * (n - 1)) welp

	const std::vector<powe::GameObjectID>& GetColliderLayer(uint32_t layer) const;
	const std::vector<powe::GameObjectID>& GetAllColliders() const;

	void AddCollider(powe::GameObjectID owner, uint32_t layer);
	void RemoveCollider(powe::GameObjectID owner);

private:

	std::array<std::vector<powe::GameObjectID>, OverlapLayer::Count> m_ColliderLayer;
	std::vector<powe::GameObjectID> m_Colliders;
};

