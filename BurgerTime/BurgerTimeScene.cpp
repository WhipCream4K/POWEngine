#include "BurgerTimeScene.h"

#include "BurgerTimeComponents.h"
#include "PlayerInputSystem.h"
#include <powengine.h>

void BurgerTimeScene::Start(const SharedPtr<powe::Core>& ,
	const SharedPtr<powe::WorldEntity>& worldEntity)
{
	using namespace powe;

	worldEntity->AddSystem<PlayerInputSystem>(PipelineLayer::Update);
	
	const SharedPtr<GameObject> testSystem{std::make_shared<GameObject>(*worldEntity)};
	testSystem->AddComponent(InputComponent{});
}

void BurgerTimeScene::Run(const SharedPtr<powe::WorldEntity>& ,
	const SharedPtr<powe::WorldClock>& )
{

}

