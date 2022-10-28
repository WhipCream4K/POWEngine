#include "EngineStatsSystem.h"
#include <POWEngine/ImGUI/imgui.h>

void EngineStatsSystem::OnUpdate(float, powe::GameObjectID)
{
    ImGui::ShowDemoWindow();
}
