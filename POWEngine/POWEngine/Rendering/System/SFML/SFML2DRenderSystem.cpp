#include "pch.h"
#include "SFML2DRenderSystem.h"

#include "POWEngine/Rendering/Components/Sprite/SFML/SFMLSpriteComponent.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
#include "POWEngine/Core/Components/Transform2D.h"

powe::SFML2DRenderSystem::SFML2DRenderSystem()
{
	DEFINE_SYSTEM_KEY(Transform2D,SpriteComponent,SFMLSpriteComponent);
}

void powe::SFML2DRenderSystem::OnDraw(const SFML2DRenderer& renderer, GameObjectID)
{
	const auto& [transform2D, sprite, sfmlSprite] = 
		GetComponentsView<Transform2D, SpriteComponent, SFMLSpriteComponent>();

	const glm::vec2 position{ transform2D->GetWorldPosition() };
	const glm::vec2 scale{ transform2D->GetWorldScale() };
	const float rotation{ transform2D->GetWorldRotation() };

	//const glm::fvec2 spriteOrigin{ sprite->GetOrigin() };

	sf::RenderStates renderStates{};
	auto& sfTransform{ renderStates.transform };

	sfTransform.translate({ position.x,position.y });
	sfTransform.rotate(sf::degrees(rotation));
	sfTransform.scale({ scale.x,scale.y });

	renderer.SubmitDrawSprite(&sfmlSprite->sprite, renderStates, int(sprite->GetZDepth()));
}

