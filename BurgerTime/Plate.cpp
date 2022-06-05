#include "Plate.h"
#include "POWEngine/Core/GameObject/GameObject.h"

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Debug/DebugRectangle.h"
#include "Rect2DCollider.h"
#include "ColliderCommand.h"
#include "BurgerTimeComponents.h"
#include "StaticSceneData.h"

SharedPtr<powe::GameObject> Plate::Create(powe::WorldEntity& worldEntity, const PlateDesc& desc)
{
	using namespace powe;
	SharedPtr<GameObject> gameObject{ std::make_shared<GameObject>(worldEntity) };

	const auto& staticSceneData{ Instance<StaticSceneData>() };

	Transform2D* transform2D = gameObject->AddComponent(Transform2D{ gameObject }, ComponentFlag::Sparse);
	transform2D->SetWorldPosition(desc.position);

	const glm::fvec2 size{ desc.size * burger::SpriteScale};

	gameObject->AddComponent(Rect2DCollider{
		worldEntity,gameObject->GetID(),desc.colliderManager,size,OverlapLayer::Static });


#ifdef _DEBUG
	gameObject->AddComponent(DebugRectangle{ gameObject,size }, ComponentFlag::Sparse);
#endif

	const int maxServingNeeded{ staticSceneData->GetPlateServingPiecesCount(desc.currentLevel) };
	PlateComponent* plateComponent = gameObject->AddComponent(PlateComponent{});
	plateComponent->maxStackCount = maxServingNeeded;
	plateComponent->realIngredientSize = size;

	return gameObject;
}
