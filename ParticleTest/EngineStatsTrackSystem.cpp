#include "EngineStatsTrackSystem.h"

#include "EngineStatsComponent.h"
#include "POWEngine/Renderer/RenderAPI.h"

#include <imgui.h>


EngineStatsTrackSystem::EngineStatsTrackSystem()
{
    // DEFINE_SYSTEM_KEY(int,std::string);
    DefineSystemKeys<EngineStatsComponent>();
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
    
    ImGui::Begin("EngineStats");
    ImGui::DragInt("FPS",&engineStats->fps);
    ImGui::DragFloat("DeltaTime",&deltaTime);
    ImGui::End();
}

