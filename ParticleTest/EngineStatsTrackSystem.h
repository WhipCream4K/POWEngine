#pragma once

#include "poweSystem.h"

class EngineStatsTrackSystem : public powe::SystemBase // default current render api
{
public:
    
    EngineStatsTrackSystem();
    void OnUpdate(float, powe::GameObjectID) override;
    // void OnDraw(const powe::RenderAPI& renderer, powe::GameObjectID id) override;
private:
    
};
