#pragma once

#include <poweSystem.h>

#include "WalkableTiles.h"

class Rect2DCollider;
struct CanWalkOnTile;
struct ObjectExtent;

namespace powe
{
	class Transform2D;
}

enum class MoveDir;

class MovementBlocking : public powe::SystemBase
{
public:

	MovementBlocking(int currentLevelIdx);

protected:

	void OnUpdate(float, powe::GameObjectID) override;

private:

	void HandleHorizontalMovement(int direction,
		powe::Transform2D* transform2D,
		CanWalkOnTile* canWalkOnTile,
		Rect2DCollider* collider) const;

	void HandleVerticalMovement(int direction,
		powe::Transform2D* transform2D,
		CanWalkOnTile* canWalkOnTile,
		Rect2DCollider* collider) const;

	bool IsOutOfBound(int direction,float pos,float against) const;
	bool IsAligned(float posY, float againstY,float epsilon) const;

	std::vector<TileData> m_CurrentTileData{};
	int m_MaxColTile{};
	int m_MaxRowTile{};
};

