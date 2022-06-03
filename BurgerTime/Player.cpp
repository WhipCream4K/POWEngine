#include "Player.h"

#include <powengine.h>

#include "AssetManager.h"
#include "BurgerTimeComponents.h"
#include "StaticVariables.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
#include "StaticSceneData.h"
#include "POWEngine/Core/Components/InputComponent.h"
#include "PlayerCommands.h"
#include "POWEngine/Rendering/Components/Debug/DebugRectangle.h"

SharedPtr<powe::GameObject> Player::Create(powe::WorldEntity& worldEntity, const PlayerDescriptor& playerDescriptor)
{
	using namespace powe;

	const auto& staticSceneData{ Instance<StaticSceneData>() };

	SharedPtr<GameObject> gameObject{ std::make_shared<GameObject>(worldEntity) };
	Transform2D* transform2D = gameObject->AddComponent(Transform2D{ gameObject });
	transform2D->SetWorldPosition(playerDescriptor.spawnPos);
	transform2D->SetWorldScale(burger::SpriteScale);

	const SpriteComponent* spriteComponent = gameObject->AddComponent(SpriteComponent{ gameObject }, ComponentFlag::Sparse);
	spriteComponent->SetTexture(*Instance<AssetManager>()->GetAsset<Texture>(burger::MainObjectSprite));
	spriteComponent->SetRect({ 0.0f,0.0f,16.0f,16.0f });
	spriteComponent->SetRenderOrder(burger::RenderOrder::Player);

	gameObject->AddComponent(AnimationComponent{ 3,0.5f });

	const float speed{ 80.0f };
	gameObject->AddComponent(Speed{ speed });

	CanWalkOnTile* canWalkOnTile{ gameObject->AddComponent(CanWalkOnTile{playerDescriptor.currentLevelIdx}) };

	const glm::ivec2 playerStartTile{ staticSceneData->GetPlayerStartTile(playerDescriptor.currentLevelIdx) };

	canWalkOnTile->currentCol = playerStartTile.x;
	canWalkOnTile->currentRow = playerStartTile.y;

	const glm::fvec2 charSize{ 16.0f,16.0f };
	gameObject->AddComponent(CharacterSize{ charSize });

	InputComponent* inputComp = gameObject->AddComponent(InputComponent{ playerDescriptor.playerIndex });
	inputComp->AddAxisCommand("Horizontal", std::make_shared<HorizontalMovement>());
	inputComp->AddAxisCommand("Vertical", std::make_shared<VerticalMovement>());

	//DelayedMovement* delayedMovement = gameObject->AddComponent(DelayedMovement{});
	//delayedMovement->timeToReachNextSplit = (6.0f * burger::SpriteScale.x) / speed;

#ifdef _DEBUG

	DebugRectangle* debugRect{ gameObject->AddComponent(DebugRectangle{gameObject},ComponentFlag::Sparse) };
	debugRect->SetSize(charSize);

#endif

	return gameObject;
}
