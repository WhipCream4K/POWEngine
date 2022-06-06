#include "EnemyWalkPatternSystem.h"
#include "BurgerTimeComponents.h"
#include "EnemyComponent.h"
#include "StaticSceneData.h"
#include "utils.h"
#include "POWEngine/Core/Components/Transform2D.h"

EnemyWalkPatternSystem::EnemyWalkPatternSystem(int levelIdx)
{
	using namespace powe;

	DEFINE_SYSTEM_KEY(Transform2D,EnemyTag, Speed, CanWalkOnTile);

	const auto& staticSceneData{ Instance<StaticSceneData>() };
	m_CurrentTileData = staticSceneData->GetAllTileInLevel(levelIdx);
	m_MaxColTile = staticSceneData->GetMaxColTile();
	m_MaxRowTile = staticSceneData->GetMaxRowTile();
}

void EnemyWalkPatternSystem::OnUpdate(float deltaTime, powe::GameObjectID)
{
	using namespace powe;

	const auto& [transform2D, enemyTag, speed, canWalkOnTile] = 
		GetComponentsView<Transform2D, EnemyTag, Speed, CanWalkOnTile>();

	if(!enemyTag->isStunned)
	{
		const auto movementDir = canWalkOnTile->movementDetails.currentMovementDir;

		int horizontal{};
		int vertical{};
		switch (movementDir)
		{
		case MoveDir::None:	break;
		case MoveDir::Left:		horizontal = 1;		break;
		case MoveDir::Right:	horizontal = -1;	break;
		case MoveDir::Up:		vertical = -1;		break;
		case MoveDir::Down:		vertical = 1;		break;
		default: ;
		}

		const int currentCol{ canWalkOnTile->currentCol  };
		const int currentRow{ canWalkOnTile->currentRow };


		auto oldPos{ transform2D->GetWorldPosition() };

		if(currentRow + 1 < m_MaxRowTile)
		{
			const TileData nextTileData{ m_CurrentTileData[(currentRow + 1) * m_MaxColTile + currentCol] };
			if(nextTileData.tileType == TileType::Ladder)
			{
				if(IsAligned(oldPos.y,nextTileData.position.y,8.0f))
				{
					canWalkOnTile->movementDetails.currentMovementDir = MoveDir::Down;
					oldPos.y += speed->climbSpeed * deltaTime;
					canWalkOnTile->movementDetails.futurePos = oldPos;
					return;
				}
			}
		}

		if(currentCol >= 0 && currentCol < m_MaxColTile && currentRow >= 0 && currentRow < m_MaxRowTile)
		{
			const TileData currentTileData{ m_CurrentTileData[currentRow * m_MaxColTile + currentCol] };

			if(currentTileData.tileType != TileType::LadderStart)
			{

				oldPos.x += float(horizontal) * speed->speed;
				oldPos.y += float(vertical) * speed->climbSpeed;

				canWalkOnTile->movementDetails.futurePos = oldPos * deltaTime;
			}
			else
			{
				if(IsAligned(oldPos.y,currentTileData.position.y,4.0f))
				{
					canWalkOnTile->movementDetails.currentMovementDir = MoveDir::Up;
					oldPos.y += speed->climbSpeed * deltaTime;
					canWalkOnTile->movementDetails.futurePos = oldPos;
				}
				else
				{
					oldPos.x += float(horizontal) * speed->speed;
					oldPos.y += float(vertical) * speed->climbSpeed;

					canWalkOnTile->movementDetails.futurePos = oldPos * deltaTime;
				}
			}
		}

		// Handle next column
		if(currentCol + horizontal < 0)
		{
			canWalkOnTile->movementDetails.currentMovementDir = MoveDir::Right;
			oldPos.x += -1.0f * speed->speed * deltaTime;
			canWalkOnTile->movementDetails.futurePos = oldPos;
			return;
		}
		if(currentCol + horizontal >= m_MaxColTile)
		{
			canWalkOnTile->movementDetails.currentMovementDir = MoveDir::Left;
			oldPos.x += float(horizontal) * speed->speed * deltaTime;
			canWalkOnTile->movementDetails.futurePos = oldPos;
			return;
		}
		if(currentRow + vertical < 0 || currentRow + vertical >= m_MaxRowTile)
		{
			canWalkOnTile->movementDetails.currentMovementDir = MoveDir::Right;
			oldPos.x += speed->speed * deltaTime;
			canWalkOnTile->movementDetails.futurePos = oldPos;
		}
	}
}
