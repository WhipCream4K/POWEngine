#include "TestCommand.h"

#include "POWEngine/Logger/LoggerUtils.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

void TestCommand::Execute(powe::WorldEntity& worldEntity,float deltaTime, powe::GameObjectID id, float axisValue)
{
	using namespace powe;

	const std::string ey{ std::to_string(axisValue) };
	POWLOGNORMAL(ey);

	Transform2D* transform2D{ worldEntity.GetComponent<Transform2D>(id) };

	glm::fvec2 horizontal{ transform2D->GetWorldPosition() };
	horizontal.x += axisValue * deltaTime * 150.0f;
	transform2D->SetWorldPosition(horizontal);
}

void FireCommand::Execute(powe::WorldEntity& , float , powe::GameObjectID , InputEvent )
{
	POWLOGNORMAL("Some");
}
