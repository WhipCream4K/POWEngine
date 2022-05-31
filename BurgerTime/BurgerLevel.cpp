#include "BurgerLevel.h"

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Core/GameObject/GameObject.h"

SharedPtr<powe::GameObject> BurgerLevel::Create(powe::WorldEntity& worldEntity, const glm::fvec2& position,int )
{
	using namespace powe;
	SharedPtr<GameObject> levelObject{ std::make_shared<GameObject>(worldEntity) };
	Transform2D* transform2D = levelObject->AddComponent(Transform2D{ levelObject },ComponentFlag::Sparse);
	transform2D->SetWorldPosition(position);

	return levelObject;
}

