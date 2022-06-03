#pragma once

#include <poweSystem.h>
#include "POWEngine/Math/Math.h"

class RectColliderDetectionSystem : public powe::SystemBase
{
public:

	RectColliderDetectionSystem();

protected:

	void OnUpdate(float, powe::GameObjectID) override;

private:

	static bool IsOverlap(const glm::fvec2& pos1, const glm::fvec2& extent1,
		const glm::fvec2& pos2, const glm::fvec2& extent2) ;
};

