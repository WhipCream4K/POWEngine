#include "pch.h"
#include "SFML2DRenderSystem.h"

#include <SFML/Graphics.hpp>

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Renderer/Components/Sprite/SFML/SFMLSpriteComponent.h"

powe::SFML2DRenderSystem::SFML2DRenderSystem()
{
	DEFINE_SYSTEM_KEY(Transform2D,SFMLSpriteComponent);
}

void powe::SFML2DRenderSystem::OnDraw(const SFML2DRenderer& renderer, GameObjectID)
{
	const auto& [transform2D, sfmlDraw] = 
		GetComponentsView<Transform2D, SFMLSpriteComponent>();

	const glm::vec2 position{ transform2D->GetWorldPosition() };
	const glm::vec2 scale{ transform2D->GetWorldScale() };
	const float rotation{ transform2D->GetWorldRotation() };

	sf::RenderStates renderStates{};
	auto& sfTransform{ renderStates.transform };

	sfTransform.translate({ position.x,position.y });
	sfTransform.rotate(sf::degrees(rotation));
	sfTransform.scale({ scale.x,scale.y });

	renderer.SubmitDrawSprite(&sfmlDraw->sprite, renderStates, sfmlDraw->drawOrder);
}

