#pragma once

#include <poweSystem.h>
#include "POWEngine/Math/Math.h"

class PlayerRespawnSystem : public powe::SystemBase
{
public:

	PlayerRespawnSystem(int levelIdx);

protected:

	void OnUpdate(float, powe::GameObjectID) override;
private:

	glm::fvec2 m_MainPlayerSpawnPos;
	float m_MaxSpawnTime{};
};

