#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"
// #include "POWEngine/Core/GameObject/GameObject.h"
#include <future>

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Math/Math.h"

struct PositionComponent : powe::Component<PositionComponent>
{
    glm::fvec2 position{};
};

struct VelocityComponent : powe::Component<VelocityComponent>
{
    glm::fvec2 linearVelocity{1.0f,1.0f};
    float maxVelocity{};
};

namespace powe
{
    class GameObject;
}

struct SceneComponent : powe::Component<SceneComponent>
{
    std::vector<SharedPtr<powe::GameObject>> agentObjects{};
    std::vector<SharedPtr<powe::GameObject>> asyncObjects{};
};

struct Steering
{
    glm::fvec2 steeringForce{};
    float weightSteering{};
};

struct WanderComponent : powe::Component<WanderComponent> , Steering
{
};

struct FleeComponent : powe::Component<FleeComponent> , Steering
{
    float fleePower{};
};

struct DebugSteeringComponent : powe::Component<DebugSteeringComponent>
{
    glm::fvec4 boundArea{};
    int activeAgents{};
    float agentSize{1.5f};
};

struct AsyncTag : powe::Component<AsyncTag>
{
    int padding[sizeof(int)]{};
};

struct AsyncRender : powe::Component<AsyncRender>
{
    
    std::future<glm::fvec2> transformUpdate{};
    std::mutex taskLock{};
};
