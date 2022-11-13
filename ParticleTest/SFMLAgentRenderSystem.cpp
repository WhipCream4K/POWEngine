#include "SFMLAgentRenderSystem.h"

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Window/Window.h"
#include "POWEngine/Core/GameObject/GameObject.h"

using namespace powe;


SFMLAgentRenderSystem::SFMLAgentRenderSystem()
{
    DefineSystemKeys<powe::Transform2D,DrawAsset>();
}

void SFMLAgentRenderSystem::OnDraw(const powe::SFML2DRenderer& renderer, const powe::Window& renderWindow,
                                   powe::GameObjectID)
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

    // needs to be valid
    // SFMLDrawComponent& derivedDraw{*reinterpret_cast<SFMLDrawComponent*>(
    //     GetComponentAddress(sfmlDraw.derivedTypeID,sfmlDraw.derivedSize))};
    
    renderer.SubmitDrawSprite(drawAsset.drawAsset, renderStates, 1);
}

