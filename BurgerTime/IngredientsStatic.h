#pragma once

#include <poweCustomtype.h>

#include "PrefabDesc.h"
#include "POWEngine/Math/Math.h"
#include "StaticVariables.h"

namespace powe
{
	class Texture;
	class GameObject;
	class WorldEntity;
}



class IngredientsStatic
{
public:

	static std::vector<SharedPtr<powe::GameObject>> Create(powe::WorldEntity&, const IngredientsDesc& desc);

private:

	static SharedPtr<powe::GameObject> CreateSubGameObject(
		powe::WorldEntity& worldEntity,
		const SharedPtr<powe::Texture>& texture,
		const glm::fvec2& position,
		const IngredientsDesc& desc,
		const SpriteInfo& spriteInfo);
};

