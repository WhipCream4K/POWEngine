#include "TestScene.h"

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
    constexpr int objectAmount{10000};
    
    // Scene Component
    const SharedPtr<GameObject> sceneData{std::make_shared<GameObject>(world)};
    m_SceneObject = sceneData;
    SceneComponent* sceneComp{sceneData->AddComponent(SceneComponent{})};

    // Initialize Debug options
    sceneData->AddComponent(Transform2D{sceneData});

    DebugSteeringComponent* debugComp{sceneData->AddComponent(DebugSteeringComponent{})};
    
    const glm::fvec2 boundStartDim{640.0f, 480.f};

    debugComp->boundArea.x = 0.0f;
    debugComp->boundArea.y = 0.0f;
    debugComp->boundArea.z = boundStartDim.x;
    debugComp->boundArea.w = boundStartDim.y;

    debugComp->activeAgents = objectAmount;

    debugComp->agentShape = std::make_unique<sf::CircleShape>();
    
    debugComp->agentShape->setRadius(1.5f);
    debugComp->agentShape->setOrigin(1.5f,1.5f);
    debugComp->agentShape->setFillColor(sf::Color::White);

    debugComp->boundAreaShape = std::make_unique<sf::RectangleShape>();
    debugComp->boundAreaShape->setFillColor(sf::Color{0,0,0,0});
    debugComp->boundAreaShape->setSize({boundStartDim.x,boundStartDim.y});
    debugComp->boundAreaShape->setOrigin({boundStartDim.x * 0.5f,boundStartDim.y * 0.5f});
    debugComp->boundAreaShape->setOutlineColor(sf::Color::Green);
    debugComp->boundAreaShape->setOutlineThickness(1.5f);

    DrawAsset* boundAreaDraw = sceneData->AddComponent(DrawAsset{});
    boundAreaDraw->drawAsset = debugComp->boundAreaShape.get();

    // Initialize Agent
    constexpr  glm::fvec2 limitHorizontal{-640.0f * 0.5f, 640.0f * 0.5f};
    constexpr glm::fvec2 limitVertical{-480.0f * 0.5f, 480.0f * 0.5f};

    for (int i = 0; i < objectAmount; ++i)
    {
        const SharedPtr<GameObject> steeringAgent{std::make_shared<GameObject>(world)};

        DrawAsset* draw_asset = steeringAgent->AddComponent(DrawAsset{});
        draw_asset->drawAsset = debugComp->agentShape.get();
        
        Transform2D* transform2D{steeringAgent->AddComponent(Transform2D{steeringAgent})};

        const glm::fvec2 randPos{
            Random::RandFloat(
                limitHorizontal.x, limitHorizontal.y),
            Random::RandFloat(limitVertical.x, limitVertical.y)
        };

        // transform2D->position = randPos;
        transform2D->SetWorldPosition(randPos);
        
        VelocityComponent* velocity = steeringAgent->AddComponent(VelocityComponent{});
        velocity->maxVelocity = Random::RandFloat(25.0f, 100.0f);

        WanderComponent* wander{steeringAgent->AddComponent(WanderComponent{})};
        wander->weightSteering = 0.2f;

        FleeComponent* flee{steeringAgent->AddComponent(FleeComponent{})};
        flee->weightSteering = 0.8f;
        flee->fleePower = 1.5f;
        
        sceneComp->agentObjects.emplace_back(steeringAgent);

        // async draw object
        {
            // const SharedPtr<GameObject> drawObject{std::make_shared<GameObject>(world)};
            // drawObject->AddComponent(Transform2D{drawObject});            
            //
            // SFML2DCircle* circleShape{
            //     drawObject->AddComponent(
            //         SFML2DCircle{world, drawObject->GetID()})
            // };
            //
            // circleShape->SetSize({1.5f, 1.5f});
            //
            // drawObject->AddComponent(AsyncRender{});
            //
            // sceneComp->drawObjects.emplace_back(drawObject);
        }
    }


    
    
    world.RegisterSystem(PipelineLayer::InputValidation, SceneControlSystem{});

    world.RegisterSystem(PipelineLayer::Update,BlendedSteeringSystem{});

    world.RegisterSystem(PipelineLayer::PostUpdate, BoundAreaSystem{sceneData});

}
