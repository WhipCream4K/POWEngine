#include "Plate.h"
#include "POWEngine/Core/GameObject/GameObject.h"

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Debug/DebugRectangle.h"
#include "Rect2DCollider.h"
#include "ColliderCommand.h"

SharedPtr<powe::GameObject> Plate::Create(powe::WorldEntity& worldEntity, const PlateDesc& desc)
{
	using namespace powe;
	SharedPtr<GameObject> gameObject{ std::make_shared<GameObject>(worldEntity) };

	gameObject->AddComponent(Transform2D{ gameObject },ComponentFlag::Sparse);

	const glm::fvec2 size{};

	Rect2DCollider* rect2DCollider = gameObject->AddComponent(Rect2DCollider{
		worldEntity,gameObject->GetID(),desc.colliderManager,size,OverlapLayer::Static },ComponentFlag::Sparse);

	rect2DCollider->OnEnterCallback = std::make_shared<PlateTriggerEnter>();
	

#ifdef _DEBUG

	gameObject->AddComponent(DebugRectangle{ gameObject,size },ComponentFlag::Sparse);

#endif

	


	return gameObject;
}
