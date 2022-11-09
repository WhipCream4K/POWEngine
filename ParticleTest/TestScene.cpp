#include "TestScene.h"

#include "powengine.h"
#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Random/Random.h"
#include "POWEngine/Renderer/Components/Debug2D/SFML/SFML2DShapeComponent.h"

using namespace powe;

TestScene::TestScene(powe::WorldEntity& world)
{

    const int objectAmount{500};
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
    
        circleShape->SetSize({10.0f,10.0f});

        SteeringComponent* steering = wanderObject->AddComponent(SteeringComponent{});
        steering->maxVelocity = Random::RandFloat(50.0f,200.0f);

        AddGameObject(wanderObject);
    }

    // Initialize bound area
    const SharedPtr<GameObject> boundArea{std::make_shared<GameObject>(world)};
    boundArea->AddComponent(Transform2D{boundArea});

    SFML2DRectangle* rectangle{boundArea->AddComponent(
        SFML2DRectangle{world,boundArea->GetID()},
        ComponentFlag::Sparse
        )};

    rectangle->SetFillColor({0,0,0,0});
    rectangle->SetOutColor({0,255,0,255});
    rectangle->SetSize({640.0,480.0f});
    rectangle->SetOutlineThickness(1.5f);
    rectangle->drawOrder = -1;
    
    
    AddGameObject(boundArea);
    // AddGameObject(testShape);
}
