#include "IngredientDynamic.h"

#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"

#include "AssetManager.h"
#include "Rect2DCollider.h"
#include "StaticSceneData.h"
#include "POWEngine/Rendering/Components/Debug/DebugRectangle.h"

SharedPtr<powe::GameObject> IngredientDynamic::Create(powe::WorldEntity& worldEntity, const IngredientDynamicDesc& desc)
{
	using namespace powe;

	SharedPtr<GameObject> gameObject{ std::make_shared<GameObject>(worldEntity) };

	const auto& assetManager{ Instance<AssetManager>() };

	const auto& staticSceneData{ Instance<StaticSceneData>() };
	
	Transform2D* transform2D = gameObject->AddComponent(Transform2D{});
	transform2D->SetWorldPosition(desc.position);
	transform2D->SetWorldScale(burger::SpriteScale);


	const SpriteInfo spriteInfo{ staticSceneData->GetIngredientSpriteInfo(desc.type) };

	SpriteComponent* spriteComp = gameObject->AddComponent(SpriteComponent{gameObject},ComponentFlag::Sparse);
	spriteComp->SetTexture(*assetManager->GetAsset<Texture>(burger::MainObjectSprite));
	spriteComp->SetRect(spriteInfo.rect);

	const glm::fvec2 colliderSize{spriteInfo.rect.z,spriteInfo.rect.w};

	
	gameObject->AddComponent(Rect2DCollider{
	worldEntity,
	gameObject->GetID(),
	desc.colliderManager,
	colliderSize * burger::SpriteScale,
	OverlapLayer::Ingredients,
	OverlapLayer::Player
		});


#ifdef _DEBUG

	gameObject->AddComponent(DebugRectangle{ gameObject,colliderSize * burger::SpriteScale }, ComponentFlag::Sparse);

#endif

	return gameObject;
}
