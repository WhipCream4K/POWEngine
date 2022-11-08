#include "DebugSteeringSystem.h"

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"


using namespace powe;

DebugSteeringSystem::DebugSteeringSystem()
{
    DefineSystemKeys<Transform2D,SteeringComponent>();
}

void DebugSteeringSystem::OnDraw(const powe::SFML2DRenderer& , const powe::Window& ,
                                 powe::GameObjectID )
{
}
