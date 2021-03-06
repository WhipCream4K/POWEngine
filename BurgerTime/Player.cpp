#include "Player.h"

#include <powengine.h>

#include "AssetManager.h"
#include "AudioManager.h"
#include "AudioManager.h"
#include "BurgerTimeComponents.h"
#include "ColliderCommand.h"
#include "OnPlayerDead.h"
#include "OnPlayerThrowPepper.h"
#include "StaticVariables.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
#include "StaticSceneData.h"
#include "POWEngine/Core/Components/InputComponent.h"
#include "PlayerCommands.h"
#include "Rect2DCollider.h"
#include "POWEngine/Rendering/Components/Debug/DebugRectangle.h"
#include "PlaySoundOnEvent.h"
#include "HUDDisplay.h"

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
	spriteComponent->SetTint(playerDescriptor.tint);	

	gameObject->AddComponent(AnimationComponent{ 3,0.5f });

	//const float speed{ 95.0f };
	Speed* speedComp = gameObject->AddComponent(Speed{});
	speedComp->speed = 90.0f;
	speedComp->climbSpeed = 75.0f;

	CanWalkOnTile* canWalkOnTile{ gameObject->AddComponent(CanWalkOnTile{playerDescriptor.currentLevelIdx}) };

	const glm::ivec2 playerStartTile{ staticSceneData->GetPlayerStartTile(playerDescriptor.currentLevelIdx) };

	canWalkOnTile->currentCol = playerStartTile.x;
	canWalkOnTile->currentRow = playerStartTile.y;

	const glm::fvec2 charSize{ 16.0f,16.0f };

	InputComponent* inputComp = gameObject->AddComponent(InputComponent{ playerDescriptor.playerIndex });
	inputComp->AddAxisCommand("Horizontal", std::make_shared<HorizontalMovement>());
	inputComp->AddAxisCommand("Vertical", std::make_shared<VerticalMovement>());
	inputComp->AddActionCommand("Fire", std::make_shared<ThrowPepper>());

	Rect2DCollider* rect2DCollider = gameObject->AddComponent(Rect2DCollider{ worldEntity,
	gameObject->GetID(),
	playerDescriptor.colliderManager,
	charSize,
	OverlapLayer::Player });

	rect2DCollider->OnEnterCallback = std::make_shared<EnemyTrigger>();

	PlayerTag* playerTag = gameObject->AddComponent(PlayerTag{}, ComponentFlag::Sparse);
	playerTag->OnPlayerThrowPepper = std::make_shared<OnPlayerThrowPepper>();
	playerTag->OnPlayerDead = std::make_shared<OnPlayerDead>();
	playerTag->playerIndex = playerDescriptor.playerIndex;

	// register audio listener
	AudioManager* audioManager = worldEntity.FindUniqueComponent<AudioManager>();
	if(audioManager)
	{
		playerTag->OnPlayerThrowPepper->Attach(audioManager->OnEventHappened.get());
		playerTag->OnPlayerDead->Attach(audioManager->OnEventHappened.get());
	}

	DisplayManager* displayManager{ worldEntity.FindUniqueComponent<DisplayManager>() };
	if(displayManager)
	{
		playerTag->OnPlayerDead->Attach(displayManager->LivesDisplay.get());
	}


#ifdef _DEBUG

	DebugRectangle* debugRect{ gameObject->AddComponent(DebugRectangle{gameObject},ComponentFlag::Sparse) };
	debugRect->SetSize(charSize);
	debugRect->SetOutlineThickness(0.5f);

#endif

	return gameObject;
}
