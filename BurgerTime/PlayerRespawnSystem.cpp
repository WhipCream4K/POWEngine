#include "PlayerRespawnSystem.h"

#include "BurgerTimeComponents.h"
#include "StaticSceneData.h"
#include "POWEngine/Core/Components/Transform2D.h"

PlayerRespawnSystem::PlayerRespawnSystem(int levelIdx)
	: m_MainPlayerSpawnPos()
	, m_MaxSpawnTime(3.0f)
{
	using namespace powe;
	DEFINE_SYSTEM_KEY(PlayerTag, Transform2D);
	const auto levelData = Instance<StaticSceneData>()->GetLevelData(levelIdx);
	m_MainPlayerSpawnPos = levelData.playerSpawnPoints;
}

void PlayerRespawnSystem::OnUpdate(float deltaTime, powe::GameObjectID)
{
	using namespace powe;

	PlayerTag* playerTag{ GetComponent<PlayerTag>() };
	if (playerTag->isDead)
	{
		if (playerTag->respawnCounter < m_MaxSpawnTime)
		{
			playerTag->respawnCounter += deltaTime;
		}
		else
		{
			playerTag->respawnCounter = 0.0f;
			playerTag->isDead = false;

			// Set player to spawn point
			Transform2D* transform2D = GetComponent<Transform2D>();
			const glm::fvec2 midScreen{ 640.0f,360.0f };
			transform2D->SetWorldPosition(m_MainPlayerSpawnPos * burger::SpriteScale + midScreen);
		}
	}
}
