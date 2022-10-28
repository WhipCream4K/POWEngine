#include "EngineStatsTrackSystem.h"

#include "EngineStatsComponent.h"
#include "POWEngine/Renderer/RenderAPI.h"

#include <imgui.h>


EngineStatsTrackSystem::EngineStatsTrackSystem()
{
    DEFINE_SYSTEM_KEY(EngineStatsComponent);
}

void EngineStatsTrackSystem::OnUpdate(float deltaTime, powe::GameObjectID)
{
    auto engineStats = GetComponent<EngineStatsComponent>();
    
    ++engineStats->fpsCounter;
    engineStats->fpsTimeStamp += deltaTime;

    
    if(engineStats->fpsTimeStamp > 1.0f)
    {
        engineStats->fpsTimeStamp -= 1.0f;
        engineStats->fps = engineStats->fpsCounter;
        engineStats->fpsCounter = 0;
    }

    int fps{engineStats->fps};
    
    ImGui::Begin("EngineStats");
    ImGui::DragInt("FPS",&fps);
    ImGui::DragFloat("DeltaTime",&deltaTime);
    ImGui::End();
    
    // ImGui::Begin("New_Window");
    // ImGui::End()
}

