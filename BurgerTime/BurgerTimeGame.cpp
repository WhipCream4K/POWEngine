#include "BurgerTimeGame.h"

#include "BurgerTimeComponents.h"
#include "PlayerInputSystem.h"
#include "POWEngine/Core/Components/AudioComponent.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Resources/Texture/Texture.h"
#include "POWEngine/Core/Input/InputSystem.h"
#include "StaticVariables.h"
#include "AssetManager.h"
#include "TestScene.h"

#include <powengine.h>

void BurgerTimeGame::Start(const SharedPtr<powe::Core>& ,
							const SharedPtr<powe::WorldEntity>& worldEntity )
{
	using namespace powe;

	//SharedPtr<PlayerInputSystem> system{ std::make_shared<PlayerInputSystem>() };
	//worldEntity->AddSystem(PipelineLayer::Update, system);

	//const SharedPtr<GameObject> testObject{std::make_shared<GameObject>(*worldEntity)};
	//m_GameObjectNodes.emplace_back(testObject);

	//testObject->AddComponent(AudioComponent{"./Resources/Sound/Jump.wav"});
	//Transform2D* transform2D = testObject->AddComponent(Transform2D{testObject});
	//transform2D->SetLocalPosition({ 640.0f,390.0f });

	//SpriteComponent* sprite{ testObject->AddComponent(SpriteComponent(testObject), ComponentFlag::Sparse) };
	//m_MainTexture = std::make_shared<Texture>("./Resources/Sprites/BurgerTime_Main.png");
	//sprite->SetTexture(*m_MainTexture);

	//const SharedPtr<GameObject> testObject2{ std::make_shared<GameObject>(*worldEntity) };
	//transform2D = testObject2->AddComponent(Transform2D{ testObject2 });

	//transform2D->SetParent(testObject);
	//m_GameObjectNodes.emplace_back(testObject2);

	Instance<AssetManager>()->RegisterAsset(burger::MainSprite, 
		std::make_shared<powe::Texture>("./Resources/Sprites/BurgerTime_Main.png"));

	m_PlayScene = std::make_shared<TestScene>();

	m_PlayScene->LoadScene(*worldEntity);


	auto& inputSetting{ worldEntity->GetInputSettings() };

	inputSetting.AddAxisMapping("Horizontal", {
		{InputDevice::D_Keyboard, Keyboard::A,-1.0f },
		{InputDevice::D_Keyboard,Keyboard::D,1.0f}
		});

	inputSetting.AddActionMapping("Fire", {
		{InputDevice::D_Keyboard,Keyboard::Space}
		});

	worldEntity->AddSystem(PipelineLayer::InputValidation, std::make_shared<InputSystem>());
}

void BurgerTimeGame::Run(const SharedPtr<powe::WorldEntity>& ,
	const SharedPtr<powe::WorldClock>& )
{
	using namespace powe;
	// manages Scene here. Save the gameobject which manages the states throughout the scene
	//if(m_Test < 1)
	//{
	//	++m_Test;
	//}
	//else
	//{
	//	m_PlayScene->UnloadScene(*worldEntity);
	//}
}

