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


void TestScene::LoadScene(powe::WorldEntity& worldEntity)
{
	using namespace powe;

	const auto& spriteTest{ std::make_shared<GameObject>(worldEntity) };

	SpriteComponent* spriteComp = spriteTest->AddComponent(SpriteComponent{ spriteTest }, ComponentFlag::Sparse);

	spriteComp->SetTexture(*Instance<AssetManager>()->GetAsset<Texture>(burger::MainSprite));
	spriteComp->SetRect({ 0.0f,0.0f,16.0f,16.0f });

	spriteTest->AddComponent(AnimationComponent{ 3,0.5f });
	
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

	const auto& inputTest{ std::make_shared<GameObject>(worldEntity) };
	InputComponent* inputComponent = inputTest->AddComponent(InputComponent{});
	inputComponent->AddAxisCommand("Horizontal", std::make_shared<TestCommand>());
	inputComponent->AddActionCommand("Fire", InputEvent::IE_Pressed, std::make_shared<FireCommand>());

	AddGameObject(inputTest);

	AddSystem(system);
}
