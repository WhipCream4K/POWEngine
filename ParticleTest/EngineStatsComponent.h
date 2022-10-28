#pragma once

#include "poweComponent.h"

struct EngineStatsComponent : public powe::Component<EngineStatsComponent>
{
    float fpsTimeStamp{};
    float timeSinceCount{};
    int fpsCounter{};
    int fps{};
};
