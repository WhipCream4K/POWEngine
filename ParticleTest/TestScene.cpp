#include "TestScene.h"

#include "powengine.h"
#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Renderer/Components/Debug2D/SFML/SFML2DShapeComponent.h"

using namespace powe;

TestScene::TestScene(powe::WorldEntity& world)
{
    const SharedPtr<GameObject> testShape{std::make_shared<GameObject>(world)};
    testShape->AddComponent(Transform2D{testShape});

    SFML2DCircle* circleShape{testShape->AddComponent(
        SFML2DCircle{world,testShape->GetID()},
        ComponentFlag::Sparse)};
    
    circleShape->SetSize({10.0f,10.0f});

    SteeringComponent* steering = testShape->AddComponent(SteeringComponent{});
    steering->linearVelocity = glm::fvec2{1.0f,1.0f};

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
    AddGameObject(testShape);
}
