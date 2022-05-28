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

	Instance<AssetManager>()->RegisterAsset(burger::MainSprite, 
		std::make_shared<powe::Texture>("./Resources/Sprites/BurgerTime_Main.png"));

	m_PlayScene = std::make_shared<TestScene>();

	m_PlayScene->LoadScene(*worldEntity);

	auto& inputSetting{ worldEntity->GetInputSettings() };

	inputSetting.AddAxisMapping("Horizontal", {
		{InputDevice::D_Keyboard, Keyboard::Left,-1.0f },
		{InputDevice::D_Keyboard,Keyboard::Right,1.0f}
		});

	inputSetting.AddAxisMapping("Vertical", {
		{InputDevice::D_Keyboard,Keyboard::Up,-1.0f},
		{InputDevice::D_Keyboard,Keyboard::Down,1.0f}
		});

	inputSetting.AddActionMapping("Fire", {
		{InputDevice::D_Keyboard,Keyboard::Space}
		});

	worldEntity->RegisterSystem(PipelineLayer::InputValidation, std::make_shared<InputSystem>());
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

