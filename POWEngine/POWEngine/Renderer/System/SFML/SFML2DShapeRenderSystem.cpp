#include "pch.h"
#include "SFML2DShapeRenderSystem.h"

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Renderer/Components/Debug/SFML/SFMLDebugRectangleComponent.h"

powe::SFML2DShapeRenderSystem::SFML2DShapeRenderSystem()
{
    DEFINE_SYSTEM_KEY(Transform2D,SFMLDebugShape);
}

void powe::SFML2DShapeRenderSystem::OnDraw(const int& renderer, GameObjectID id)
{
    
}

// void powe::SFML2DShapeRenderSystem::OnDraw(const powe::SFML2DRenderer& renderer, powe::GameObjectID)
// {
//     const auto& [transform2D, sfmlDraw] =
//     GetComponentsView<Transform2D, SFMLDebugShape>();
//
//     const glm::vec2 position{ transform2D->GetWorldPosition() };
//     const float rotation{ transform2D->GetWorldRotation() };
//
//     sf::RenderStates renderStates{};
//     auto& sfTransform{ renderStates.transform };
//
//     sfTransform.translate({ position.x,position.y });
//     sfTransform.rotate(glm::degrees(rotation));
//
//     //sfTransform.scale({ scale.x,scale.y });
//
//     renderer.SubmitDrawSprite(sfmlDraw->shape, renderStates, sfmlDraw->drawOrder);
// }
