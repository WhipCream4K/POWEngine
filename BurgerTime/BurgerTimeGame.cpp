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

#include "GameState.h"
#include "PlayScene.h"
#include "POWEngine/Rendering/Resources/Font/Font.h"

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

	Instance<StaticSceneData>()->Initialize();

	// Initialize persistent GameObject
	const auto dynamicSceneData{ std::make_shared<GameObject>(*worldEntity) };
	m_SceneDataID = dynamicSceneData->GetID();
	dynamicSceneData->AddComponent(DynamicSceneData{ m_SceneDataID });

	//m_MenuScene = std::make_shared<MenuScene>(m_SceneDataID);
	//m_MenuScene->LoadScene(*worldEntity);

	// Initialize Game State
	{
		m_MainGameState = GameState::GameStart;
	}


	// Initialize Main input data
	{
		auto& inputSetting{ worldEntity->GetInputSettings() };

		inputSetting.AddAxisMapping("Horizontal", {
			{InputDevice::D_Keyboard, Keyboard::Left,-1.0f },
			{InputDevice::D_Keyboard,Keyboard::Right,1.0f},
			{InputDevice::D_Gamepad,GamepadKey::GPK_Left_AxisX,1.0f}
			});

		inputSetting.AddAxisMapping("Vertical", {
			{InputDevice::D_Keyboard,Keyboard::Up,-1.0f},
			{InputDevice::D_Keyboard,Keyboard::Down,1.0f},
			{InputDevice::D_Gamepad,GamepadKey::GPK_Left_AxisY,-1.0f}
			});

		inputSetting.AddAxisMapping("MenuVertical", {
			{InputDevice::D_Keyboard,Keyboard::Up,-1.0f},
			{InputDevice::D_Keyboard,Keyboard::Down,1.0f}
			});

		inputSetting.AddAxisMapping("Select", {
			{InputDevice::D_Keyboard,Keyboard::Enter,1.0f}
			});

		inputSetting.AddActionMapping("Fire", {
			{InputDevice::D_Keyboard,Keyboard::Space}
			});
	}


	worldEntity->RegisterSystem(PipelineLayer::InputValidation, std::make_shared<InputSystem>());
}

void BurgerTimeGame::Run(const SharedPtr<powe::WorldEntity>& worldEntity,
	const SharedPtr<powe::WorldClock>& worldClock)
{
	using namespace powe;

	const float deltaTime{ worldClock->GetDeltaTime() };

	if(m_MainGameState)
	{
		const auto& oldState{ m_MainGameState };
		const auto newState{ m_MainGameState->HandleInput(*worldEntity, m_SceneDataID) };

		if(oldState != newState)
		{
			oldState->Exit(*worldEntity, m_SceneDataID);
			newState->Enter(*worldEntity, m_SceneDataID);
			m_MainGameState = newState;
		}

		m_MainGameState->Update(*worldEntity, deltaTime, m_SceneDataID);
	}
}

