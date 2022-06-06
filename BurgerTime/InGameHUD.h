#pragma once

#include <poweCustomtype.h>
#include "PrefabDesc.h"

struct DisplayManager;

namespace powe
{
	class WorldEntity;
	class GameObject;
}

class InGameHUD
{
public:

	static std::vector<SharedPtr<powe::GameObject>> Create(powe::WorldEntity&, const IngameHUDDesc& desc);

	static SharedPtr<powe::GameObject> CreateDisplayManager(powe::WorldEntity&, const IngameHUDDesc& desc);

private:

	static std::vector<SharedPtr<powe::GameObject>> CreateSubObject(powe::WorldEntity&, const IngameHUDDesc& desc);
	static SharedPtr<powe::GameObject> CreateHealthDisplay(powe::WorldEntity&,const glm::fvec2& position);
	static SharedPtr<powe::GameObject> CreateTextDisplay(
		powe::WorldEntity&, 
		const glm::fvec2& position,
		int textSize,
		const std::string&,
		const glm::uvec4& color);

	static void CreateHUDElements(powe::WorldEntity&,powe::GameObjectID id,DisplayManager*, const IngameHUDDesc& desc,int textDir);
	
};

