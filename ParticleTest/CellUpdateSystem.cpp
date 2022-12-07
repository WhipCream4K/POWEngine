#include "CellUpdateSystem.h"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "UserComponents.h"


class CellRenderer
{
public:
    void SubmitDrawCommands();
private:
    sf::RenderTexture m_RenderTexture{};
    sf::Drawable* m_AgentDrawAsset{};
    std::vector<glm::fvec2> m_Position{};
    sf::Sprite m_FinalSprite{};
};

void CellRenderer::SubmitDrawCommands()
{
    sf::RenderStates renderStates{};
    
    for (const auto& pos: m_Position)
    {
        renderStates.transform.translate(pos.x,pos.y);
        m_RenderTexture.draw(*m_AgentDrawAsset,renderStates);
    }

    // Submit draw sprite
    m_RenderTexture.display();
    m_FinalSprite.setTexture(m_RenderTexture.getTexture());
    // Call Render command list and submit it
}

CellUpdateSystem::CellUpdateSystem()
{
}

void CellUpdateSystem::OnUpdate(float, powe::GameObjectID)
{
    CellUpdaterComponent& cellUpdater{ GetComponent<CellUpdaterComponent>()};

    GetWorld().GetThreadPool()->PushMemberTask(&CellRenderer::SubmitDrawCommands,m_CellRenderers[1]);
    
}
