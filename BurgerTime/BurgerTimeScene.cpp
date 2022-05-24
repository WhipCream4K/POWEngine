#include "BurgerTimeScene.h"

#include "BurgerTimeComponents.h"
#include "PlayerInputSystem.h"
#include "POWEngine/Core/Components/AudioComponent.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include <powengine.h>

#include "POWEngine/Rendering/Resources/Texture/Texture.h"

void BurgerTimeScene::Start(const SharedPtr<powe::Core>& ,
							const SharedPtr<powe::WorldEntity>& worldEntity)
{
	using namespace powe;

	//SharedPtr<SystemBase> system = worldEntity->AddSystem<PlayerInputSystem>(PipelineLayer::Update).lock();
	SharedPtr<PlayerInputSystem> system{ std::make_shared<PlayerInputSystem>() };
	worldEntity->AddSystem(PipelineLayer::Update, system);

	const SharedPtr<GameObject> testObject{std::make_shared<GameObject>(*worldEntity)};
	m_GameObjectNodes.emplace_back(testObject);


	testObject->AddComponent(AudioComponent{"./Resources/Sound/Jump.wav"});
	Transform2D* transform2D = testObject->AddComponent(Transform2D{});

	transform2D->SetLocalPosition({ 640.0f,390.0f });

	SpriteComponent* sprite{ testObject->AddComponent(SpriteComponent(testObject), ComponentFlag::Sparse) };
	m_MainTexture = std::make_shared<Texture>("./Resources/Sprites/BurgerTime_Main.png");
	sprite->SetTexture(*m_MainTexture);
}

void BurgerTimeScene::Run(const SharedPtr<powe::WorldEntity>& ,
	const SharedPtr<powe::WorldClock>& )
{

}

