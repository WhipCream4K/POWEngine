#include "BurgerTimeGame.h"

#include <powengine.h>

#include "BurgerTimeComponents.h"
#include "PlayerInputSystem.h"
#include "POWEngine/Core/Components/AudioComponent.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Resources/Texture/Texture.h"
#include "POWEngine/Core/Clock/WorldClock.h"
#include "POWEngine/Core/Input/InputSystem.h"
#include "StaticVariables.h"
#include "AssetManager.h"
#include "TestScene.h"
#include "MenuScene.h"
#include "StaticSceneData.h"
#include "DebugControllerSystem.h"

#include "GameState.h"
#include "PlayScene.h"
#include "POWEngine/Rendering/Resources/Font/Font.h"
#include "ColliderResolver.h"
#include "RectColliderDetectionSystem.h"
#include "AudioManager.h"
#include "MenuState.h"
#include "GameStateSystem.h"

void BurgerTimeGame::Start(const SharedPtr<powe::Core>&,
						   const SharedPtr<powe::WorldEntity>& worldEntity)
{
	using namespace powe;

	const auto assetManager{ Instance<AssetManager>() };

	assetManager->RegisterAsset(burger::MainObjectSprite,
		std::make_shared<powe::Texture>("./Resources/Sprites/BurgerTime_Main.png"));

	assetManager->RegisterAsset(burger::MainFont,
		std::make_shared<Font>("./Resources/Fonts/VCR_OSD_MONO_1.001.ttf"));

	assetManager->RegisterAsset(burger::MainLevelSprite,
		std::make_shared<Texture>("./Resources/Sprites/BurgerTime_Stages.png"));

	assetManager->RegisterAsset(burger::MenuPointer,
		std::make_shared<powe::Texture>("./Resources/Sprites/Pointer.png"));

	const auto healthTexture{ std::make_shared<Texture>("./Resources/Sprites/HealthLogo.png") };
	healthTexture->SetRepeated(true);

	assetManager->RegisterAsset(burger::HealthSprite,healthTexture);

	Instance<StaticSceneData>()->Initialize();

	// Initialize persistent GameObject
	const auto dynamicSceneData{ std::make_shared<GameObject>(*worldEntity) };
	m_SceneDataID = dynamicSceneData->GetID();

	DynamicSceneData* dynamicScene = dynamicSceneData->AddComponent(DynamicSceneData{ m_SceneDataID });
	dynamicScene->currentGameState = GameState::GameStart;
	dynamicScene->timeBeforeGameStart = 3.5f;
	dynamicScene->maxLevel = 3;
	
	const auto audioManager{ std::make_shared<GameObject>(*worldEntity) };
	audioManager->AddComponent(AudioManager{});

	//m_MenuScene = std::make_shared<TestScene>();
	//m_MenuScene->LoadScene(*worldEntity);

	// Initialize Game State
	{
		//m_MainGameState = GameState::GameStart;
	}

	// Initialize Main input data
	{
		auto& inputSetting{ worldEntity->GetInputSettings() };

		inputSetting.AddAxisMapping("Horizontal", {
			{InputDevice::D_Keyboard, Keyboard::Left,-1.0f },
			{InputDevice::D_Keyboard,Keyboard::Right,1.0f},
			{InputDevice::D_Gamepad,GamepadKey::GPK_DPAD_Left,-1.0f},
			{InputDevice::D_Gamepad,GamepadKey::GPK_DPAD_Right,1.0f}
			});

		inputSetting.AddAxisMapping("Vertical", {
			{InputDevice::D_Keyboard,Keyboard::Up,-1.0f},
			{InputDevice::D_Keyboard,Keyboard::Down,1.0f},
			{InputDevice::D_Gamepad,GamepadKey::GPK_DPAD_Up,-1.0f},
			{InputDevice::D_Gamepad,GamepadKey::GPK_DPAD_Down,1.0f}
			});

		inputSetting.AddAxisMapping("MenuVertical", {
			{InputDevice::D_Keyboard,Keyboard::Up,-1.0f},
			{InputDevice::D_Keyboard,Keyboard::Down,1.0f},
			{InputDevice::D_Gamepad,GamepadKey::GPK_DPAD_Up,-1.0f},
			{InputDevice::D_Gamepad,GamepadKey::GPK_DPAD_Down,1.0f},
			});

		inputSetting.AddActionMapping("Select", {
			{InputDevice::D_Keyboard,Keyboard::Enter},
			{InputDevice::D_Gamepad,GamepadKey::GPK_A}
			});

		inputSetting.AddActionMapping("ShowDebug", {
			{InputDevice::D_Keyboard,Keyboard::F8}
			});

		inputSetting.AddActionMapping("SkipStage", {
	{InputDevice::D_Keyboard,Keyboard::F6}
			});

		inputSetting.AddActionMapping("Fire", {
			{InputDevice::D_Keyboard,Keyboard::Z},
			{InputDevice::D_Gamepad,GamepadKey::GPK_B}
			});

		inputSetting.AddActionMapping("SkipToMenu", {
			{InputDevice::D_Keyboard,Keyboard::F7}
			});

		inputSetting.AddActionMapping("ForceDead", {
	{InputDevice::D_Keyboard,Keyboard::F5}
			});

	}


	SharedPtr<InputSystem> inputSystem{ std::make_shared<InputSystem>() };
	dynamicScene->blockingSystem[PipelineLayer::InputValidation].emplace_back(inputSystem);

	worldEntity->RegisterSystem(PipelineLayer::InputValidation, inputSystem);
	worldEntity->RegisterSystem(PipelineLayer::PhysicsValidation, std::make_shared<RectColliderDetectionSystem>());
	worldEntity->RegisterSystem(PipelineLayer::Update, std::make_shared<DebugControllerSystem>());
	worldEntity->RegisterSystem(PipelineLayer::Update, std::make_shared<GameStateSystem>());
}

void BurgerTimeGame::Run(const SharedPtr<powe::WorldEntity>& ,
	const SharedPtr<powe::WorldClock>& )
{
	using namespace powe;

	// was going to do the scene changes here but later decide to put it in system instead...
}

