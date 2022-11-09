#include "DebugSteeringSystem.h"

#include <imgui-SFML.h>
#include <imgui.h>

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Renderer/Components/Debug2D/SFML/SFML2DShapeComponent.h"


using namespace powe;

DebugSteeringSystem::DebugSteeringSystem()
{
    DefineSystemKeys<DebugSteeringComponent>();
}

void DebugSteeringSystem::OnUpdate(float, powe::GameObjectID)
{
    DebugSteeringComponent* debugOptions{ GetComponent<DebugSteeringComponent>()};
    
    ImGui::Begin("Debug Window");

    if(ImGui::DragFloat4("Bounding Box",&debugOptions->boundArea.x))
    {
        SFML2DRectangle* rectangle{GetComponent<SFML2DRectangle>()};
        rectangle->SetSize({debugOptions->boundArea.z,debugOptions->boundArea.w});
    }

    ImGui::End();
}
