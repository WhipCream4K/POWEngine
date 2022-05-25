#include "BurgerTimeGame.h"

#include "BurgerTimeComponents.h"
#include "PlayerInputSystem.h"
#include "POWEngine/Core/Components/AudioComponent.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Resources/Texture/Texture.h"
#include "AssetManager.h"

#include <powengine.h>

void BurgerTimeGame::Start(const SharedPtr<powe::Core>& ,
							const SharedPtr<powe::WorldEntity>& )
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

	Instance<AssetManager>()->RegisterAsset("BurgerTime_MainSprite", 
		std::make_shared<powe::Texture>("./Resources/Sprites/BurgerTime_Main.png"));

	SharedPtr<powe::Texture> some{ Instance<AssetManager>()->GetAsset<powe::Texture>("BurgerTime_MainSprite")};
}

void BurgerTimeGame::Run(const SharedPtr<powe::WorldEntity>& ,
	const SharedPtr<powe::WorldClock>& )
{
	// manages Scene here. Save the gameobject which manages the states throughout the scene
	
}

