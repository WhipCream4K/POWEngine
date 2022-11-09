#include "TestScene.h"

#include <imgui-SFML.h>
#include <imgui.h>

#include "AgentOptionsUpdate.h"
#include "BoundAreaSystem.h"
#include "DebugSteeringSystem.h"
#include "powengine.h"
#include "UserComponents.h"
#include "WanderingSteeringSystem.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Random/Random.h"
#include "POWEngine/Renderer/Components/Debug2D/SFML/SFML2DShapeComponent.h"

using namespace powe;

TestScene::TestScene(powe::WorldEntity& world)
{
    
    const int objectAmount{4000};
    const glm::fvec2 limitHorizontal{-640.0f * 0.5f,640.0f * 0.5f};
    const glm::fvec2 limitVertical{-480.0f * 0.5f,480.0f * 0.5f};
    
    for (int i = 0; i < objectAmount; ++i)
    {
        const SharedPtr<GameObject> wanderObject{std::make_shared<GameObject>(world)};
        Transform2D* transform{ wanderObject->AddComponent(Transform2D{wanderObject})};

        const glm::fvec2 randPos{Random::RandFloat(
            limitHorizontal.x,limitHorizontal.y),
            Random::RandFloat(limitVertical.x,limitVertical.y)};

        transform->SetWorldPosition(randPos);
    
        SFML2DCircle* circleShape{wanderObject->AddComponent(
        SFML2DCircle{world,wanderObject->GetID()},ComponentFlag::Sparse)};
    
        circleShape->SetSize({1.5f,1.5f});

        VelocityComponent* steering = wanderObject->AddComponent(VelocityComponent{});
        steering->maxVelocity = Random::RandFloat(50.0f,200.0f);

        AddGameObject(wanderObject);
    }

    // Initialize Debug options
    const SharedPtr<GameObject> debugOpt{std::make_shared<GameObject>(world)};
    debugOpt->AddComponent(Transform2D{debugOpt});
    
    DebugSteeringComponent* debugComp{ debugOpt->AddComponent(DebugSteeringComponent{})};

    const glm::fvec2 boundStartDim{640.0f,480.f};
    
    debugComp->boundArea.x = 0.0f;
    debugComp->boundArea.y = 0.0f;
    debugComp->boundArea.z = boundStartDim.x;
    debugComp->boundArea.w = boundStartDim.y;

    SFML2DRectangle* rectangle{debugOpt->AddComponent(SFML2DRectangle{world,debugOpt->GetID()}
        ,ComponentFlag::Sparse)};

    rectangle->SetFillColor({0,0,0,0});
    rectangle->SetOutColor({0,255,0,255});
    rectangle->SetSize(boundStartDim);
    rectangle->SetOutlineThickness(1.5f);
    rectangle->drawOrder = -1;
    
    AddGameObject(debugOpt);
    
    world.RegisterSystem(PipelineLayer::PostUpdate,std::make_shared<BoundAreaSystem>(debugOpt));

    world.RegisterSystem(PipelineLayer::Update,std::make_shared<WanderingSteeringSystem>());

    world.RegisterSystem(PipelineLayer::InputValidation,std::make_shared<DebugSteeringSystem>());
    world.RegisterSystem(PipelineLayer::InputValidation,std::make_shared<AgentOptionsUpdate>(debugOpt));
    
}
