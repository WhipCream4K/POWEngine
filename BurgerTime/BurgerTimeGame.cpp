#include "BurgerTimeGame.h"

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

#include <powengine.h>

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

	// Initialize persistent GameObject
	const auto dynamicSceneData{ std::make_shared<GameObject>(*worldEntity) };
	m_SceneDataID = dynamicSceneData->GetID();
	dynamicSceneData->AddComponent(DynamicSceneData{ m_SceneDataID });

	m_MenuScene = std::make_shared<MenuScene>(m_SceneDataID);
	m_MenuScene->LoadScene(*worldEntity);

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

	inputSetting.AddActionMapping("Select", {
		{InputDevice::D_Keyboard,Keyboard::Enter}
		});

	inputSetting.AddActionMapping("Fire", {
		{InputDevice::D_Keyboard,Keyboard::Space}
		});

	worldEntity->RegisterSystem(PipelineLayer::InputValidation, std::make_shared<InputSystem>());
}

void BurgerTimeGame::Run(const SharedPtr<powe::WorldEntity>&,
	const SharedPtr<powe::WorldClock>&)
{
	using namespace powe;

	//const float delta{ worldClock->GetDeltaTime() };

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

