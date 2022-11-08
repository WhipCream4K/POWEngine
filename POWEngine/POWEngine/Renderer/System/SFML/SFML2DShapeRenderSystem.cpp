#include "pch.h"
#include "SFML2DShapeRenderSystem.h"

#include <iostream>

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Renderer/Components/Debug2D/SFML/SFML2DShapeComponent.h"

powe::SFML2DShapeRenderSystem::SFML2DShapeRenderSystem()
{
    //DEFINE_SYSTEM_KEY(Transform2D,SFMLDebugShape);
    DefineSystemKeys<Transform2D, SFML2DShapeComponent>();
}

// void powe::SFML2DShapeRenderSystem::OnDraw(const powe::SFML2DRenderer& renderer, powe::GameObjectID)
// {
//     const auto& [transform2D, sfmlShape] =
//         GetComponentsView<Transform2D, SFML2DShapeComponent>();
//
//     const glm::vec2 position{transform2D->GetPosition()};
//     const float rotation{transform2D->GetRotation()};
//
//     sf::RenderStates renderStates{};
//     auto& sfTransform{renderStates.transform};
//
//     sfTransform.translate({position.x, position.y});
//     sfTransform.rotate(glm::degrees(rotation));
//     
//     // upcast to shape interface
//     SFML2DShapeComponent* shape{GetComponentByID<SFML2DShapeComponent>(sfmlShape->derivedType)};
//     renderer.SubmitDrawSprite(shape->GetSFMLShape(), renderStates, sfmlShape->drawOrder);
// }
