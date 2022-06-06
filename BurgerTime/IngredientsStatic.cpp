#include "IngredientsStatic.h"

#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "Rect2DCollider.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
#include "POWEngine/Rendering/Components/Debug/DebugRectangle.h"
#include "BurgerTimeComponents.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "ColliderCommand.h"
#include "OnIngredientStepped.h"
#include "PlaySoundOnEvent.h"
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
		glm::fvec2 startPos{ desc.position };
		glm::fvec2 ingredientSize{ spriteInfo.rect.z / float(stepCount),spriteInfo.rect.w / float(stepCount)};
		ingredientSize *= burger::SpriteScale;

		glm::fvec4 subSpriteRect{ spriteInfo.rect };
		const float splitSpriteWidth{ spriteInfo.rect.z / float(stepCount) };

		startPos.x = (startPos.x - (ingredientSize.x + (ingredientSize.x / 2.0f)));
		subSpriteRect.z = splitSpriteWidth;

		for (int i = 0; i < stepCount; ++i)
		{
			SharedPtr<GameObject> sub{ CreateSubGameObject(worldEntity, mainTexture, startPos, desc, subSpriteRect) };
			outGameObject.emplace_back(sub);
			startPos.x += ingredientSize.x;
			subSpriteRect.x += splitSpriteWidth;
		}
	}


	return outGameObject;
}

SharedPtr<powe::GameObject> IngredientsStatic::CreateSubGameObject(
	powe::WorldEntity& worldEntity,
	const SharedPtr<powe::Texture>& texture,
	const glm::fvec2& position,
	const IngredientsDesc& desc,
	const glm::fvec4& spriteRect)
{
	using namespace powe;

	SharedPtr<GameObject> sub1{ std::make_shared<GameObject>(worldEntity) };
	const glm::fvec2 colliderSize{ spriteRect.z,spriteRect.w };

	Transform2D* transform2D = sub1->AddComponent(Transform2D{ sub1 });
	transform2D->SetWorldPosition(position);
	transform2D->SetWorldScale(burger::SpriteScale);

	const SpriteComponent* spriteComp = sub1->AddComponent(SpriteComponent{ sub1 }, ComponentFlag::Sparse);
	spriteComp->SetTexture(*texture);
	spriteComp->SetRenderOrder(burger::RenderOrder::PickUps);
	spriteComp->SetRect(spriteRect);

	Rect2DCollider* rect2DCollider = sub1->AddComponent(Rect2DCollider{
		worldEntity,
		sub1->GetID(),
		desc.colliderManager,
		colliderSize * burger::SpriteScale,
		OverlapLayer::Ingredients,
		OverlapLayer::Player | OverlapLayer::Dynamic
		});

	rect2DCollider->OnEnterCallback = std::make_shared<OnStaticIngredientTrigger>();

	StepHandler* stepHandler = sub1->AddComponent(StepHandler{});
	stepHandler->stepHandlerID = desc.stepHandler;
	stepHandler->OnIngredientStepped = std::make_shared<OnIngredientStepped>();

	const AudioManager* audioManager = worldEntity.FindUniqueComponent<AudioManager>();
	if(audioManager)
	{
		stepHandler->OnIngredientStepped->Attach(audioManager->OnEventHappened.get());
	}

#ifdef _DEBUG

	sub1->AddComponent(DebugRectangle{sub1,colliderSize * burger::SpriteScale}, ComponentFlag::Sparse);

#endif

	return sub1;
}
