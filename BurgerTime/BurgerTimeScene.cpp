#include "BurgerTimeScene.h"

#include "BurgerTimeComponents.h"
#include "PlayerInputSystem.h"
#include "POWEngine/Core/Components/AudioComponent.h"
#include <powengine.h>

void BurgerTimeScene::Start(const SharedPtr<powe::Core>& ,
	const SharedPtr<powe::WorldEntity>& worldEntity)
{
	using namespace powe;

	//SharedPtr<SystemBase> system = worldEntity->AddSystem<PlayerInputSystem>(PipelineLayer::Update).lock();
	SharedPtr<PlayerInputSystem> system{ std::make_shared<PlayerInputSystem>() };
	worldEntity->AddSystem(PipelineLayer::Update, system);
	const SharedPtr<GameObject> testSystem{std::make_shared<GameObject>(*worldEntity)};
	testSystem->AddComponent(AudioComponent{"./Resources/Sound/Jump.wav"},ComponentFlag::Sparse);
}

void BurgerTimeScene::Run(const SharedPtr<powe::WorldEntity>& ,
	const SharedPtr<powe::WorldClock>& )
{

}

