#include "TestScene.h"

#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Rendering/Resources/Texture/Texture.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
#include "POWEngine/Core/Components/AudioComponent.h"
#include "POWEngine/Core/Components/Transform2D.h"

#include "PlayerInputSystem.h"
#include "BurgerTimeComponents.h"
#include "AnimationSystem.h"

#include "AssetManager.h"
#include "StaticVariables.h"
#include "POWEngine/Core/Components/InputComponent.h"
#include "TestCommand.h"
#include "BurgerTimeComponents.h"
#include "ColliderCommand.h"
#include "PlayerCommands.h"
#include "POWEngine/Rendering/Components/Text/TextComponent.h"
#include "POWEngine/Rendering/Components/Debug/DebugRectangle.h"
#include "Rect2DCollider.h"
#include "ColliderResolver.h"
#include "ColliderCommand.h"
#include "RectColliderDetectionSystem.h"
#include "FallingSystem.h"


void TestScene::LoadScene(powe::WorldEntity& worldEntity)
{
	using namespace powe;

	const auto& spriteTest{ std::make_shared<GameObject>(worldEntity) };

	m_Player = spriteTest;

	SpriteComponent* spriteComp = spriteTest->AddComponent(SpriteComponent{ spriteTest }, ComponentFlag::Sparse);

	spriteComp->SetTexture(*Instance<AssetManager>()->GetAsset<Texture>(burger::MainObjectSprite));
	spriteComp->SetRect({ 0.0f,0.0f,16.0f,16.0f });

	const glm::fvec2 boundSize{16.0f,16.0f};

	spriteTest->AddComponent(AnimationComponent{ 3,0.5f });
	spriteTest->AddComponent(Speed{ 150.0f });
	spriteTest->AddComponent(DebugRectangle{ spriteTest ,boundSize * burger::SpriteScale}, ComponentFlag::Sparse);


	Transform2D* transform2D = spriteTest->AddComponent(Transform2D{ spriteTest });

	transform2D->SetWorldPosition({ 640.0f,360.0f });
	transform2D->SetWorldScale(burger::SpriteScale);


	const auto& audioTest{ std::make_shared<GameObject>(worldEntity) };
	audioTest->AddComponent(AudioComponent{ "./Resources/Sound/Jump.wav" }, ComponentFlag::Sparse);

	//AddGameObject(spriteTest);
	AddGameObject(audioTest);

	SharedPtr<SystemBase> system{};

	system = std::make_shared<AnimationSystem>();

	worldEntity.RegisterSystem(PipelineLayer::PostUpdate, system);

	InputComponent* inputComponent = spriteTest->AddComponent(InputComponent{});

	inputComponent->AddAxisCommand("Horizontal", std::make_shared<DebugHorizontalMovement>());
	inputComponent->AddAxisCommand("Vertical", std::make_shared<DebugVerticalMovement>());

	AddSystem(system);

	const auto& textTest{ std::make_shared<GameObject>(worldEntity) };
	Transform2D* textTransform = textTest->AddComponent(Transform2D{ textTest });
	textTransform;
	//textTransform->SetParent(spriteTest);

	TextComponent* textComponent{ textTest->AddComponent(TextComponent{40,textTest},ComponentFlag::Sparse) };
	textComponent->SetFont(Instance<AssetManager>()->GetAsset<Font>(burger::MainFont));
	textComponent->SetText("Hello World");

	AddGameObject(textTest);

	const auto& levelTest{ std::make_shared<GameObject>(worldEntity) };
	Transform2D* levelTransform = levelTest->AddComponent(Transform2D{ levelTest });

	levelTransform->SetWorldPosition({ 640.0f,360.0f });
	levelTransform->SetWorldScale(burger::SpriteScale);

	spriteComp = levelTest->AddComponent(SpriteComponent{ levelTest }, ComponentFlag::Sparse);
	spriteComp->SetRenderOrder(burger::RenderOrder::Background);
	spriteComp->SetTexture(*Instance<AssetManager>()->GetAsset<Texture>(burger::MainLevelSprite));
	spriteComp->SetRect({ 0.0f,0.0f,208.0f,200.0f });

	AddGameObject(levelTest);


	const auto colliderManager{ std::make_shared<GameObject>(worldEntity) };
	colliderManager->AddComponent(ColliderResolver{});
	AddGameObject(colliderManager);

	spriteTest->AddComponent(Rect2DCollider{ spriteTest,colliderManager,boundSize,OverlapLayer::Player });
	//rect2DCollider->OnEnterCallback = std::make_shared<DebugTriggerEnter>();

	const auto someItem{ std::make_shared<GameObject>(worldEntity) };
	transform2D = someItem->AddComponent(Transform2D{someItem});
	transform2D->SetWorldPosition({ 700.0f,360.0f });

	spriteTest->AddComponent(PlayerTag{});

	someItem->AddComponent(DebugRectangle{ someItem,boundSize });
	Rect2DCollider* rect2DCollider = someItem->AddComponent(Rect2DCollider{ someItem,colliderManager,boundSize,OverlapLayer::Ingredients,OverlapLayer::Player });
	rect2DCollider->OnEnterCallback = std::make_shared<OnStaticIngredientTrigger>();


	const auto rectColliderDetection{ std::make_shared<RectColliderDetectionSystem>() };
	AddSystem(rectColliderDetection);
	worldEntity.RegisterSystem(PipelineLayer::Update, rectColliderDetection);

	const auto fallingSystem{ std::make_shared<FallingSystem>() };
	AddSystem(fallingSystem);
	worldEntity.RegisterSystem(PipelineLayer::Update, fallingSystem);
}

void TestScene::Run(powe::WorldEntity& worldEntity,float delta)
{
	worldEntity;
	delta;
	//if (m_ExpiredTime < 2.5f)
	//	m_ExpiredTime += delta;
	//else
	//	worldEntity.RemoveGameObject(m_Player->GetID());
}
