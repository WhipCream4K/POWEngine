#include "SceneDraw.h"

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"

powe::SceneDraw::SceneDraw()
{
    DefineSystemKeys<Transform2D,DrawAsset>();
}

void powe::SceneDraw::OnDraw(const SFML2DRenderer& renderer, const Window& renderWindow, GameObjectID)
{
    const auto& [transform2D,drawAsset] = GetComponentsView<Transform2D,DrawAsset>();
    
    const glm::fvec2 halfWindowDim{
        float(renderWindow.GetWidth()) * 0.5f,
        float(renderWindow.GetHeight()) * 0.5f};
    
    const glm::vec2 position{ transform2D.GetPosition() };
    
    const glm::vec2 scale{ transform2D.GetScale() };
    const float rotation{ transform2D.GetRotation() };
    
    sf::RenderStates renderStates{};
    auto& sfTransform{ renderStates.transform };
    
    // draw defaulted to center of the screen
    sfTransform.translate({ position.x + halfWindowDim.x,halfWindowDim.y - position.y });
    sfTransform.rotate(glm::degrees(rotation));
    sfTransform.scale({ scale.x,scale.y });

    renderer.SubmitRenderEntity(*drawAsset.drawAsset,renderStates);
}
