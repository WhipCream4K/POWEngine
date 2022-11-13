#include "TestScene.h"

#include "AsyncSystemTest.h"
#include "BlendedSteeringSystem.h"
#include "BoundAreaSystem.h"
#include "FleeSteeringSystem.h"
#include "SceneControlSystem.h"
#include "powengine.h"
#include "UserComponents.h"
#include "WanderingSteeringSystem.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Random/Random.h"
#include "POWEngine/Renderer/Components/Debug2D/SFML/SFML2DShapeComponent.h"

using namespace powe;

TestScene::TestScene(powe::WorldEntity& world)
{
    // Scene Component
    const SharedPtr<GameObject> sceneData{std::make_shared<GameObject>(world)};
    m_SceneObject = sceneData;
    SceneComponent* sceneComp{sceneData->AddComponent(SceneComponent{})};
    
    constexpr int objectAmount{20000};
    constexpr  glm::fvec2 limitHorizontal{-640.0f * 0.5f, 640.0f * 0.5f};
    constexpr glm::fvec2 limitVertical{-480.0f * 0.5f, 480.0f * 0.5f};

    for (int i = 0; i < objectAmount; ++i)
    {
        const SharedPtr<GameObject> steeringAgent{std::make_shared<GameObject>(world)};
        PositionComponent* position{steeringAgent->AddComponent(PositionComponent{})};

        const glm::fvec2 randPos{
            Random::RandFloat(
                limitHorizontal.x, limitHorizontal.y),
            Random::RandFloat(limitVertical.x, limitVertical.y)
        };

        position->position = randPos;

        VelocityComponent* velocity = steeringAgent->AddComponent(VelocityComponent{});
        velocity->maxVelocity = Random::RandFloat(25.0f, 100.0f);

        WanderComponent* wander{steeringAgent->AddComponent(WanderComponent{})};
        wander->weightSteering = 0.2f;

        FleeComponent* flee{steeringAgent->AddComponent(FleeComponent{})};
        flee->weightSteering = 0.8f;
        flee->fleePower = 1.5f;

        // async draw object
        {
            const SharedPtr<GameObject> drawObject{std::make_shared<GameObject>(world)};
            drawObject->AddComponent(Transform2D{drawObject});            

            SFML2DCircle* circleShape{
                drawObject->AddComponent(
                    SFML2DCircle{world, steeringAgent->GetID()})
            };

            circleShape->SetSize({1.5f, 1.5f});

            drawObject->AddComponent(AsyncRender{});

            sceneComp->asyncObjects.emplace_back(drawObject);
        }
        
        sceneComp->agentObjects.emplace_back(steeringAgent);
        
        // AddGameObject(steeringAgent);
        // AddGameObject(drawObject);
    }

    // Initialize Debug options
    sceneData->AddComponent(Transform2D{sceneData});

    DebugSteeringComponent* debugComp{sceneData->AddComponent(DebugSteeringComponent{})};

    const glm::fvec2 boundStartDim{640.0f, 480.f};

    debugComp->boundArea.x = 0.0f;
    debugComp->boundArea.y = 0.0f;
    debugComp->boundArea.z = boundStartDim.x;
    debugComp->boundArea.w = boundStartDim.y;

    debugComp->activeAgents = objectAmount;

    SFML2DRectangle* rectangle{
        sceneData->AddComponent(SFML2DRectangle{world, sceneData->GetID()}
                               , ComponentFlag::Sparse)
    };

    rectangle->SetFillColor({0, 0, 0, 0});
    rectangle->SetOutColor({0, 255, 0, 255});
    rectangle->SetSize(boundStartDim);
    rectangle->SetOutlineThickness(1.5f);
    rectangle->drawOrder = -1;

    auto asyncTest{std::make_shared<GameObject>(world)};
    asyncTest->AddComponent(AsyncTag{});

    sceneComp->asyncObjects.emplace_back(asyncTest);
    
    world.RegisterSystem(PipelineLayer::InputValidation, SceneControlSystem{});

    // world.RegisterSystem(PipelineLayer::Update,AsyncSystemTest{});
    // world.RegisterSystem(PipelineLayer::AsyncUpdate,WanderingSteeringSystem{});
    // world.RegisterSystem(PipelineLayer::AsyncUpdate,FleeSteeringSystem{});
    
    // world.RegisterSystem(PipelineLayer::PostUpdate, AgentOptionsUpdate{sceneData});
    // world.RegisterSystem(PipelineLayer::PostUpdate, BoundAreaSystem{sceneData});
    // world.RegisterSystem(PipelineLayer::Update,BlendedSteeringSystem{});
}
