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
#include "PlayerCommands.h"
#include "POWEngine/Rendering/Components/Text/TextComponent.h"
#include "POWEngine/Rendering/Components/Debug/DebugRectangle.h"


void TestScene::LoadScene(powe::WorldEntity& worldEntity)
{
	using namespace powe;

	const auto& spriteTest{ std::make_shared<GameObject>(worldEntity) };

	SpriteComponent* spriteComp = spriteTest->AddComponent(SpriteComponent{ spriteTest }, ComponentFlag::Sparse);

	spriteComp->SetTexture(*Instance<AssetManager>()->GetAsset<Texture>(burger::MainObjectSprite));
	spriteComp->SetRect({ 0.0f,0.0f,16.0f,16.0f });

	spriteTest->AddComponent(AnimationComponent{ 3,0.5f });
	spriteTest->AddComponent(PlayerSpeed{ 150.0f });
	DebugRectangle* debugRect = spriteTest->AddComponent(DebugRectangle{ spriteTest }, ComponentFlag::Sparse);

	debugRect->SetSize({ 20.0f,20.0f });

	Transform2D* transform2D = spriteTest->AddComponent(Transform2D{ spriteTest });

	transform2D->SetWorldPosition({ 640.0f,360.0f });
	transform2D->SetWorldScale(burger::SpriteScale);


	const auto& audioTest{ std::make_shared<GameObject>(worldEntity) };
	audioTest->AddComponent(AudioComponent{ "./Resources/Sound/Jump.wav" }, ComponentFlag::Sparse);

	AddGameObject(spriteTest);
	AddGameObject(audioTest);

	SharedPtr<SystemBase> system{ std::make_shared<PlayerInputSystem>() };
	worldEntity.RegisterSystem(PipelineLayer::Update, system);

	AddSystem(system);

	system = std::make_shared<AnimationSystem>();

	worldEntity.RegisterSystem(PipelineLayer::PostUpdate, system);

	InputComponent* inputComponent = spriteTest->AddComponent(InputComponent{});

	inputComponent->AddAxisCommand("Horizontal", std::make_shared<HorizontalMovement>());
	inputComponent->AddAxisCommand("Vertical", std::make_shared<VerticalMovement>());

	AddSystem(system);

	const auto& textTest{ std::make_shared<GameObject>(worldEntity) };
	Transform2D* textTransform = textTest->AddComponent(Transform2D{ textTest });
	textTransform->SetParent(spriteTest);

	TextComponent* textComponent{ textTest->AddComponent(TextComponent{40,textTest},ComponentFlag::Sparse) };
	textComponent->SetFont(Instance<AssetManager>()->GetAsset<Font>(burger::MainFont));
	textComponent->SetText("Hello World");

	AddGameObject(textTest);

	const auto& levelTest{ std::make_shared<GameObject>(worldEntity) };
	levelTest->AddComponent(Transform2D{ levelTest });
	spriteComp = levelTest->AddComponent(SpriteComponent{ levelTest }, ComponentFlag::Sparse);
	spriteComp->SetTexture(*Instance<AssetManager>()->GetAsset<Texture>(burger::MainLevelSprite));
	
	AddGameObject(levelTest);
}
