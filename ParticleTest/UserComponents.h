#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"
// #include "POWEngine/Core/GameObject/GameObject.h"
#include <future>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

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
    std::vector<SharedPtr<powe::GameObject>> drawObjects{};
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
    int padding[16]{};
};

struct DebugSteeringComponent : powe::Component<DebugSteeringComponent>
{
    glm::fvec4 boundArea{};
    int activeAgents{};
    float agentSize{1.5f};
    OwnedPtr<sf::RectangleShape> agentShape{};
    OwnedPtr<sf::RectangleShape> boundAreaShape{};
};

struct DrawAsset : powe::Component<DrawAsset>
{
    sf::Drawable* drawAsset{};
};

struct AgentVerticesBatch : powe::Component<AgentVerticesBatch>
{
    sf::VertexArray vertexBuffer{};
    float rectSize{};
    uint32_t vertexID{};
};
