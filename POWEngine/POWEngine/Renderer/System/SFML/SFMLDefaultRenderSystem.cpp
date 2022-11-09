#include "pch.h"
#include "SFMLDefaultRenderSystem.h"

#include <SFML/Graphics.hpp>
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Renderer/Components/Debug2D/SFML/SFML2DShapeComponent.h"
#include "POWEngine/Renderer/Components/SFML/SFMLDrawComponent.h"
#include "POWEngine/Window/Window.h"

powe::SFMLDefaultRenderSystem::SFMLDefaultRenderSystem()
{
    DefineSystemKeys<Transform2D,SFMLDrawComponent>();
}

void powe::SFMLDefaultRenderSystem::OnDraw(const SFML2DRenderer& renderer, const Window& renderWindow, GameObjectID)
{
    const auto& [transform2D, sfmlDraw] = 
    GetComponentsView<Transform2D, SFMLDrawComponent>();

    const glm::fvec2 halfWindowDim{
        float(renderWindow.GetWidth()) * 0.5f,
        float(renderWindow.GetHeight()) * 0.5f};
    
    const glm::vec2 position{ transform2D->GetPosition() };
    const glm::vec2 scale{ transform2D->GetScale() };
    const float rotation{ transform2D->GetRotation() };
 
    sf::RenderStates renderStates{};
    auto& sfTransform{ renderStates.transform };

    // draw defaulted to center of the screen
    sfTransform.translate({ position.x + halfWindowDim.x,halfWindowDim.y - position.y });
    sfTransform.rotate(glm::degrees(rotation));
    sfTransform.scale({ scale.x,scale.y });
 
    SFMLDrawComponent* derivedDraw{reinterpret_cast<SFMLDrawComponent*>(
        GetComponentAddress(sfmlDraw->derivedTypeID,sfmlDraw->derivedSize))};
    
    renderer.SubmitDrawSprite(derivedDraw->GetDrawable(), renderStates, sfmlDraw->drawOrder);
}


