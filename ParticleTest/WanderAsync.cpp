#include "WanderAsync.h"


glm::fvec2 WanderAsync::CalculatePosition(float deltaTime, powe::GameObjectID)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    
    SetTaskState(true);
}
