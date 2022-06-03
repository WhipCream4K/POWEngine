#include "IngredientsStatic.h"

#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "Rect2DCollider.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
#include "AssetManager.h"
#include "StaticSceneData.h"

std::vector<SharedPtr<powe::GameObject>> IngredientsStatic::Create(powe::WorldEntity& worldEntity, const IngredientsDesc& desc)
{
	using namespace powe;

	const auto& assetManager{ Instance<AssetManager>() };
	const auto& mainTexture{ assetManager->GetAsset<Texture>(burger::MainObjectSprite) };

	const auto& staticSceneData{ Instance<StaticSceneData>() };
	
	std::vector<SharedPtr<powe::GameObject>> outGameObject{};

	const SpriteInfo& spriteInfo{ staticSceneData->GetIngredientSpriteInfo(desc.ingredientsType) };

	// Create 4 subs GameObjects for physics detection
	{
		const int stepCount{ 4 };
		glm::fvec2 startPos{};
		for (int i = 0; i < stepCount; ++i)
		{
			SharedPtr<GameObject> sub{ CreateSubGameObject(worldEntity, mainTexture, startPos, desc, spriteInfo) };
			outGameObject.emplace_back(sub);
		}
	}


	return outGameObject;
}

SharedPtr<powe::GameObject> IngredientsStatic::CreateSubGameObject(
	powe::WorldEntity& worldEntity,
	const SharedPtr<powe::Texture>& texture,
	const glm::fvec2& position,
	const IngredientsDesc& desc,
	const SpriteInfo& spriteInfo)
{
	using namespace powe;

	const SharedPtr<GameObject> sub1{ std::make_shared<GameObject>(worldEntity) };

	const glm::fvec2 colliderSize{ 16.0f,16.0f };

	Transform2D* transform2D = sub1->AddComponent(Transform2D{ sub1 });
	transform2D->SetWorldPosition(position);

	const SpriteComponent* spriteComp = sub1->AddComponent(SpriteComponent{ sub1 }, ComponentFlag::Sparse);
	spriteComp->SetTexture(*texture);
	spriteComp->SetRenderOrder(burger::RenderOrder::PickUps);
	spriteComp->SetRect(spriteInfo.rect);

	sub1->AddComponent(Rect2DCollider{
		worldEntity,
		sub1->GetID(),
		desc.colliderManager,
		colliderSize,
		OverlapLayer::Ingredients,
		OverlapLayer::Player
		});

	return sub1;
}
