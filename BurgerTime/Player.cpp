#include "Player.h"

#include <powengine.h>

#include "AssetManager.h"
#include "BurgerTimeComponents.h"
#include "StaticVariables.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
//#include "POWEngine/Rendering/Resources/Texture/Texture.h"

SharedPtr<powe::GameObject> Player::Create(powe::WorldEntity& worldEntity, const glm::fvec2& position, int)
{
	using namespace powe;

	SharedPtr<GameObject> gameObject{ std::make_shared<GameObject>(worldEntity) };
	Transform2D* transform2D = gameObject->AddComponent(Transform2D{ gameObject });
	transform2D->SetWorldPosition(position);
	transform2D->SetWorldScale(burger::SpriteScale);

	const SpriteComponent* spriteComponent = gameObject->AddComponent(SpriteComponent{ gameObject }, ComponentFlag::Sparse);
	spriteComponent->SetTexture(*Instance<AssetManager>()->GetAsset<Texture>(burger::MainObjectSprite));
	spriteComponent->SetRect({ 0.0f,0.0f,16.0f,16.0f });
	spriteComponent->SetRenderOrder(burger::RenderOrder::Player);

	gameObject->AddComponent(AnimationComponent{ 3,0.5f });

	gameObject->AddComponent(Speed{ 150.0f });

	CanWalkOnTile* canWalkOnTile{ gameObject->AddComponent(CanWalkOnTile{}) };
	canWalkOnTile->currentCol = 0;
	canWalkOnTile->currentRow = 0;

	return gameObject;
}
