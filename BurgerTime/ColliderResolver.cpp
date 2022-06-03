#include "ColliderResolver.h"

#include <algorithm>

const std::vector<powe::GameObjectID>& ColliderResolver::GetColliderLayer(uint32_t layer) const
{
	return m_ColliderLayer[layer];
}

const std::vector<powe::GameObjectID>& ColliderResolver::GetAllColliders() const
{
	return m_Colliders;
}

void ColliderResolver::AddCollider(powe::GameObjectID owner, uint32_t layer)
{
	if (std::ranges::find(m_Colliders, owner) == m_Colliders.end())
		m_Colliders.emplace_back(owner);

	const uint32_t idx{OverlapLayer::LayerToIndex(layer)};
	if (std::ranges::find(m_ColliderLayer[idx], owner) == m_ColliderLayer[idx].end())
		m_ColliderLayer[idx].emplace_back(owner);
}

void ColliderResolver::RemoveCollider(powe::GameObjectID owner)
{
	const auto removeItr{ std::ranges::remove(m_Colliders,owner) };
	m_Colliders.erase(removeItr.begin(),m_Colliders.end());

	for (auto& collider : m_ColliderLayer)
	{
		const auto findItr{ std::ranges::find(collider,owner) };
		if (findItr != collider.end())
		{
			collider.erase(findItr);
			return;
		}
	}
}
