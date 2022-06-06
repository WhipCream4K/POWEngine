#include "EnemySpawnSystem.h"

#include "BurgerTimeComponents.h"
#include "StaticSceneData.h"

#include "HotDog.h"
#include "ColliderResolver.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Core/GameObject/GameObject.h"

EnemySpawnSystem::EnemySpawnSystem(int levelIdx)
{
	using namespace powe;

	DEFINE_SYSTEM_KEY(DynamicSceneData);

	m_CurrentSpawnableTile = Instance<StaticSceneData>()->GetEnemySpawnableTile(levelIdx);

	const auto& staticSceneData{ Instance<StaticSceneData>() };
	m_CurrentTileData = staticSceneData->GetAllTileInLevel(levelIdx);
	m_MaxColTile = staticSceneData->GetMaxColTile();
	m_MaxRowTile = staticSceneData->GetMaxRowTile();
}

void EnemySpawnSystem::OnUpdate(float, powe::GameObjectID)
{
	using namespace powe;

	DynamicSceneData* dynamicSceneData{ GetComponent<DynamicSceneData>() };

	if(dynamicSceneData->intervalPlayState == IntervalPlayState::Playing)
	{
		if(int(m_SpawnedEnemy.size()) < m_MaxEnemy)
		{

			EnemyDesc enemyDesc{};
			GameObjectID colliderManager{};
			GetWorld()->FindUniqueComponent<ColliderResolver>(colliderManager);

			enemyDesc.colliderManager = colliderManager;
			enemyDesc.type = EnemyType::HotDog;

			const auto enemy{ HotDog::Create(*GetWorld(),enemyDesc) };

			const int randNum{ rand() % int(m_CurrentSpawnableTile.size()) };

			const auto tile{ m_CurrentSpawnableTile[randNum] };

			const auto tileData{ m_CurrentTileData[tile.y * m_MaxColTile + tile.x] };

			CanWalkOnTile* canWalkOnTile = enemy->GetComponent<CanWalkOnTile>();
			canWalkOnTile->currentCol = tile.x;
			canWalkOnTile->currentRow = tile.y;
			canWalkOnTile->movementDetails.currentMovementDir = MoveDir::Left;

			Transform2D* transform2D = enemy->GetComponent<Transform2D>();
			if(transform2D)
			{
				transform2D->SetWorldPosition(tileData.position);
			}

			m_SpawnedEnemy.emplace_back(enemy);
		}
	}
}
