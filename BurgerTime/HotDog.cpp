#include "HotDog.h"

#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
#include "AssetManager.h"
#include "StaticVariables.h"
#include "BurgerTimeComponents.h"
#include "POWEngine/Rendering/Components/Debug/DebugRectangle.h"
#include "ColliderResolver.h"
#include "Rect2DCollider.h"

SharedPtr<powe::GameObject> HotDog::Create(powe::WorldEntity& worldEntity, const EnemyDesc& desc)
{
	using namespace powe;

	const auto& assetManager{ Instance<AssetManager>() };
	const auto& mainTexture{ assetManager->GetAsset<Texture>(burger::MainObjectSprite) };

	auto gameobject{ std::make_shared<GameObject>(worldEntity) };
	Transform2D* transform2D = gameobject->AddComponent(Transform2D{ gameobject });
	transform2D->SetWorldPosition(desc.position);
	transform2D->SetWorldScale(burger::SpriteScale);


	glm::fvec2 size{ 16.0f, 16.0f };
	size *= burger::SpriteScale;

	const glm::fvec4 spriteRect{ 16.0f * 2.0f,16.0f * 2.0f,16.0f,16.0f };
	SpriteComponent* spriteComponent = gameobject->AddComponent(SpriteComponent{ gameobject }, ComponentFlag::Sparse);
	spriteComponent->SetTexture(*mainTexture);
	spriteComponent->SetRenderOrder(burger::RenderOrder::Enemy);
	spriteComponent->SetRect(spriteRect);

	gameobject->AddComponent(CanWalkOnTile{});

	Speed* speedComp = gameobject->AddComponent(Speed{});
	speedComp->speed = 90.0f;
	speedComp->climbSpeed = 75.0f;

	// Will let the player handle the collision
	gameobject->AddComponent(Rect2DCollider{ worldEntity,
		gameobject->GetID(),desc.colliderManager,
		size,OverlapLayer::Enemy });

	EnemyTag* enemyTag = gameobject->AddComponent(EnemyTag{},ComponentFlag::Sparse);
	enemyTag->enemyType = desc.type;

#ifdef _DEBUG

	gameobject->AddComponent(DebugRectangle{ gameobject,size });

#endif

	return gameobject;
}
