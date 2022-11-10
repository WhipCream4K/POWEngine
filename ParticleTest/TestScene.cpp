﻿#include "TestScene.h"

#include <imgui-SFML.h>
#include <imgui.h>

#include "AgentOptionsUpdate.h"
#include "BlendedSteeringSystem.h"
#include "BoundAreaSystem.h"
#include "DebugSteeringSystem.h"
#include "FleeSteeringSystem.h"
#include "powengine.h"
#include "UserComponents.h"
#include "WanderingSteeringSystem.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Random/Random.h"
#include "POWEngine/Renderer/Components/Debug2D/SFML/SFML2DShapeComponent.h"

using namespace powe;

TestScene::TestScene(powe::WorldEntity& world)
{
    const int objectAmount{10000};
    const glm::fvec2 limitHorizontal{-640.0f * 0.5f, 640.0f * 0.5f};
    const glm::fvec2 limitVertical{-480.0f * 0.5f, 480.0f * 0.5f};

    for (int i = 0; i < objectAmount; ++i)
    {
        const SharedPtr<GameObject> steeringAgent{std::make_shared<GameObject>(world)};
        Transform2D* transform{steeringAgent->AddComponent(Transform2D{steeringAgent})};

        const glm::fvec2 randPos{
            Random::RandFloat(
                limitHorizontal.x, limitHorizontal.y),
            Random::RandFloat(limitVertical.x, limitVertical.y)
        };

        transform->SetWorldPosition(randPos);

        VelocityComponent* velocity = steeringAgent->AddComponent(VelocityComponent{});
        velocity->maxVelocity = Random::RandFloat(25.0f, 100.0f);

        WanderComponent* wander{steeringAgent->AddComponent(WanderComponent{})};
        wander->weightSteering = 0.2f;

        FleeComponent* flee{steeringAgent->AddComponent(FleeComponent{})};
        flee->weightSteering = 0.8f;
        flee->fleePower = 1.5f;

        // const SharedPtr<GameObject> drawObject{RenderAttach::Create(world,steeringAgent)};
        // const SharedPtr<GameObject> drawObject{std::make_shared<GameObject>(world)};

        SFML2DCircle* circleShape{
            steeringAgent->AddComponent(
                SFML2DCircle{world, steeringAgent->GetID()})
        };

        circleShape->SetSize({1.5f, 1.5f});

        AddGameObject(steeringAgent);
        // AddGameObject(drawObject);
    }

    // Initialize Debug options
    const SharedPtr<GameObject> debugOpt{std::make_shared<GameObject>(world)};
    debugOpt->AddComponent(Transform2D{debugOpt});

    DebugSteeringComponent* debugComp{debugOpt->AddComponent(DebugSteeringComponent{})};

    const glm::fvec2 boundStartDim{640.0f, 480.f};

    debugComp->boundArea.x = 0.0f;
    debugComp->boundArea.y = 0.0f;
    debugComp->boundArea.z = boundStartDim.x;
    debugComp->boundArea.w = boundStartDim.y;

    debugComp->agentAmount = objectAmount;

    SFML2DRectangle* rectangle{
        debugOpt->AddComponent(SFML2DRectangle{world, debugOpt->GetID()}
                               , ComponentFlag::Sparse)
    };

    rectangle->SetFillColor({0, 0, 0, 0});
    rectangle->SetOutColor({0, 255, 0, 255});
    rectangle->SetSize(boundStartDim);
    rectangle->SetOutlineThickness(1.5f);
    rectangle->drawOrder = -1;

    AddGameObject(debugOpt);

    world.RegisterSystem(PipelineLayer::InputValidation, DebugSteeringSystem{});
    world.RegisterSystem(PipelineLayer::InputValidation, AgentOptionsUpdate{debugOpt});
    
    world.RegisterSystem(PipelineLayer::Update,BlendedSteeringSystem{});
    
    world.RegisterSystem(PipelineLayer::PostUpdate, BoundAreaSystem{debugOpt});
}
