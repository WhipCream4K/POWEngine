#include "SFMLThreadRenderSystem.h"

#include "UserComponents.h"
#include <SFML/Graphics.hpp>
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Renderer/Components/SFML/SFMLDrawComponent.h"
#include "POWEngine/Window/Window.h"
#include <future>

using namespace powe;

SFMLThreadRenderSystem::SFMLThreadRenderSystem()
{
    DefineSystemKeys<Transform2D,SFMLDrawComponent,AsyncRender>();
}

void SFMLThreadRenderSystem::OnDraw(const powe::SFML2DRenderer& renderer, const powe::Window& renderWindow,
    powe::GameObjectID)
{
    auto& asyncRender{GetComponent<AsyncRender>()};

    bool isUpdateFinished{};
    glm::fvec2 newPosition{};
    
    {
        std::scoped_lock taskLock{asyncRender.taskLock};
        if(asyncRender.transformUpdate.valid())
        {
            if(asyncRender.transformUpdate.wait_for(std::chrono::milliseconds(1))
                == std::future_status::ready)
            {
                isUpdateFinished = true;
                newPosition = asyncRender.transformUpdate.get();
            }
        }
    }
    
    
    const auto& [transform2D, sfmlDraw] = 
    GetComponentsView<Transform2D, SFMLDrawComponent>();

    if(isUpdateFinished)
    {
        transform2D.SetWorldPosition(newPosition);
    }

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
    SFMLDrawComponent& derivedDraw{*reinterpret_cast<SFMLDrawComponent*>(
        GetComponentAddress(sfmlDraw.derivedTypeID,sfmlDraw.derivedSize))};
    
    renderer.SubmitDrawSprite(derivedDraw.GetDrawable(), renderStates, sfmlDraw.drawOrder);
}
