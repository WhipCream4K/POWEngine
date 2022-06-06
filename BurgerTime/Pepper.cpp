#include "Pepper.h"

#include "BurgerTimeComponents.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
#include "POWEngine/Rendering/Components/Debug/DebugRectangle.h"
#include "AssetManager.h"
#include "StaticVariables.h"
#include "Rect2DCollider.h"

SharedPtr<powe::GameObject> Pepper::Create(powe::WorldEntity& worldEntity, const PepperDesc& desc)
{
	using namespace powe;

	auto gameObject{ std::make_shared<GameObject>(worldEntity) };

	const auto& assetManager{ Instance<AssetManager>() };
	const auto& mainTexture{ assetManager->GetAsset<Texture>(burger::MainObjectSprite) };

	Transform2D* transform2D = gameObject->AddComponent(Transform2D{ gameObject });
	transform2D->SetWorldPosition(desc.position);
	transform2D->SetWorldScale(burger::SpriteScale);

	SpriteComponent* spriteComponent = gameObject->AddComponent(SpriteComponent{ gameObject }, ComponentFlag::Sparse);
	spriteComponent->SetTexture(*mainTexture);
	spriteComponent->SetRenderOrder(burger::RenderOrder::PickUps);

	const glm::fvec4 spriteRect{193.0f,16.0f,16.0f,16.0f};
	spriteComponent->SetRect(spriteRect);

	glm::fvec2 colliderSize{ 16.0f,16.0f };
	colliderSize *= burger::SpriteScale;

	gameObject->AddComponent(Rect2DCollider{ worldEntity,
		gameObject->GetID(),desc.colliderManager,colliderSize,
		OverlapLayer::Player,OverlapLayer::Enemy, });



#ifdef _DEBUG

	gameObject->AddComponent(DebugRectangle{ gameObject,colliderSize }, ComponentFlag::Sparse);

#endif

	return  gameObject;
}
