#include "MovementBlocking.h"

#include "BurgerTimeComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "StaticVariables.h"
#include "StaticSceneData.h"

MovementBlocking::MovementBlocking(int currentLevelIdx)
	: m_CurrentTileData()
{
	using namespace powe;
	DEFINE_SYSTEM_KEY(Transform2D, CanWalkOnTile, CharacterSize);

	const auto& staticSceneData{ Instance<StaticSceneData>() };
	m_CurrentTileData = staticSceneData->GetAllTileInLevel(currentLevelIdx);
	m_MaxColTile = staticSceneData->GetMaxColTile();
	m_MaxRowTile = staticSceneData->GetMaxRowTile();
}

void MovementBlocking::OnUpdate(float, powe::GameObjectID)
{
	using namespace powe;

	const auto& [transform2D, canWalkOnTile, charSize] = GetComponentsView<Transform2D, CanWalkOnTile, CharacterSize>();

	const MoveDir currentMoveDir{ canWalkOnTile->movementDetails.currentMovementDir };
	if (currentMoveDir != MoveDir::None)
	{
		int horizontal{};
		if (currentMoveDir == MoveDir::Left || currentMoveDir == MoveDir::Right)
		{
			horizontal = currentMoveDir == MoveDir::Left ? -1 : 1;
			HandleHorizontalMovement(horizontal, transform2D, canWalkOnTile, charSize);
		}
		else
		{
			const int vertical = currentMoveDir == MoveDir::Up ? -1 : 1;
			HandleVerticalMovement(vertical, transform2D, canWalkOnTile, charSize);
		}

		canWalkOnTile->movementDetails.currentMovementDir = MoveDir::None;
	}

}

void MovementBlocking::HandleHorizontalMovement(int direction, powe::Transform2D* transform2D,
	CanWalkOnTile* canWalkOnTile, CharacterSize* charSize) const
{
	// handle column movement
	const int nextTileCol{ canWalkOnTile->currentCol + direction };
	const int currentRow{ canWalkOnTile->currentRow };

	const auto& oldPos{ transform2D->GetWorldPosition() };

	if (nextTileCol < 0 || nextTileCol >= m_MaxColTile)
	{
		const TileData lastTileData{ m_CurrentTileData[(currentRow * m_MaxColTile) + canWalkOnTile->currentCol] };

		if (!IsAligned(oldPos.y, lastTileData.position.y, 8.0f))
			return;

		// can only walk to the nearest block
		if (!IsOutOfBound(direction, canWalkOnTile->movementDetails.futurePos.x,
			lastTileData.position.x))
		{
			transform2D->SetWorldPosition(canWalkOnTile->movementDetails.futurePos);
		}

		return;
	}

	const TileData nextTileData{ m_CurrentTileData[(currentRow * m_MaxColTile) + nextTileCol] };

	// Can't walk
	if (nextTileData.tileType == TileType::None)
	{
		// Continue with the same path
		const TileData lastTileData{ m_CurrentTileData[(currentRow * m_MaxColTile) + canWalkOnTile->currentCol] };

		if (!IsAligned(oldPos.y, lastTileData.position.y, 8.0f))
			return;

		// can only walk to the nearest block
		if (!IsOutOfBound(direction, canWalkOnTile->movementDetails.futurePos.x,
			lastTileData.position.x))
		{
			transform2D->SetWorldPosition(canWalkOnTile->movementDetails.futurePos);
		}

		return;
	}

	if (!IsAligned(oldPos.y, nextTileData.position.y, 8.0f))
		return;

	if (IsOutOfBound(direction,
		canWalkOnTile->movementDetails.futurePos.x + (charSize->size.x * float(direction)),
		nextTileData.position.x))
	{
		// Approved of the movement
		canWalkOnTile->currentCol = nextTileCol;
	}
	else
	{
		// modify so the horizontal pos is the same as the ground
		constexpr float offset{2.0f};
		canWalkOnTile->movementDetails.futurePos.y = nextTileData.position.y - (offset * burger::SpriteScale.x);
		transform2D->SetWorldPosition(canWalkOnTile->movementDetails.futurePos);
	}
}

void MovementBlocking::HandleVerticalMovement(int direction, powe::Transform2D* transform2D,
	CanWalkOnTile* canWalkOnTile, CharacterSize* charSize) const
{
	const int nextTileRow{ canWalkOnTile->currentRow + direction };
	const int currentCol{ canWalkOnTile->currentCol };

	const auto& oldPos{ transform2D->GetWorldPosition() };

	if (nextTileRow < 0 || nextTileRow >= m_MaxRowTile )
	{
		const TileData lastTileData{ m_CurrentTileData[(canWalkOnTile->currentRow * m_MaxColTile) + currentCol] };

		if (!IsAligned(oldPos.x, lastTileData.position.x, 4.0f))
			return;

		// can only walk to the nearest block
		if (!IsOutOfBound(direction, canWalkOnTile->movementDetails.futurePos.y,
			lastTileData.position.y))
		{
			transform2D->SetWorldPosition(canWalkOnTile->movementDetails.futurePos);
		}

		return;
	}

	const TileData nextTileData{ m_CurrentTileData[(nextTileRow * m_MaxColTile) + currentCol] };

	// Can't walk
	if (nextTileData.tileType == TileType::None)
	{

		const TileData lastTileData{ m_CurrentTileData[(canWalkOnTile->currentRow * m_MaxColTile) + currentCol] };

		if (!IsAligned(oldPos.x, lastTileData.position.x, 0.1f))
			return;

		// can only walk to the nearest block
		if (!IsOutOfBound(direction, canWalkOnTile->movementDetails.futurePos.y,
			lastTileData.position.y))
		{
			transform2D->SetWorldPosition(canWalkOnTile->movementDetails.futurePos);
		}

		return;
	}

	if (!IsAligned(oldPos.x, nextTileData.position.x, 4.0f))
		return;

	if (IsOutOfBound(direction,
		canWalkOnTile->movementDetails.futurePos.y + (charSize->size.y * float(direction)),
		nextTileData.position.y))
	{
		// Approved of the movement
		canWalkOnTile->currentRow = nextTileRow;
	}
	else
	{
		// modify so the horizontal pos is the same as the ladder
		canWalkOnTile->movementDetails.futurePos.x = nextTileData.position.x;
		transform2D->SetWorldPosition(canWalkOnTile->movementDetails.futurePos);
	}
}


bool MovementBlocking::IsOutOfBound(int direction, float pos, float against) const
{
	if (direction > 0)
	{
		if (pos > against)
			return true;
	}
	else
	{
		if (pos < against)
			return true;
	}

	return false;
}


bool MovementBlocking::IsAligned(float posY, float againstY, float epsilon) const
{
	if (abs(posY - againstY) <= epsilon)
		return true;

	return false;
}

