#include "BurgerLevel.h"

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Core/GameObject/GameObject.h"

#include "StaticSceneData.h"
#include "StaticVariables.h"
#include "AssetManager.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"

SharedPtr<powe::GameObject> BurgerLevel::Create(powe::WorldEntity& worldEntity, const glm::fvec2& position,int levelIdx)
{
	using namespace powe;

	const auto& assetManager{ Instance<AssetManager>() };
	const auto& staticSceneData{ Instance<StaticSceneData>() };

	SharedPtr<GameObject> levelObject{ std::make_shared<GameObject>(worldEntity) };
	Transform2D* transform2D = levelObject->AddComponent(Transform2D{ levelObject },ComponentFlag::Sparse);
	transform2D->SetWorldPosition(position);
	transform2D->SetWorldScale(burger::SpriteScale);

	LevelData levelData{ staticSceneData->GetLevelData(levelIdx) };

	SpriteComponent* spriteComp = levelObject->AddComponent(SpriteComponent{ levelObject }, ComponentFlag::Sparse);
	spriteComp->SetTexture(*assetManager->GetAsset<Texture>(burger::MainLevelSprite));
	spriteComp->SetRect(levelData.spriteRect);
	spriteComp->SetRenderOrder(burger::RenderOrder::Background);

	return levelObject;
}

