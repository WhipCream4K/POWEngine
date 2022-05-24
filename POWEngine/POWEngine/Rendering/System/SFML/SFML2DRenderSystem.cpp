#include "pch.h"
#include "SFML2DRenderSystem.h"

#include "POWEngine/Rendering/Components/Sprite/SFML/SFMLSpriteComponent.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
//#include "POWEngine/Rendering/Components/Sprite/SpriteImpl.h"

powe::SFML2DRenderSystem::SFML2DRenderSystem()
{
	DEFINE_SYSTEM_KEY(SpriteComponent,SFMLSpriteComponent);
}

void powe::SFML2DRenderSystem::OnDraw(const WorldEntity& worldEntity, const SFML2DRenderer& renderer, GameObjectID id)
{
	SpriteComponent* spriteComponent{ worldEntity.GetComponent<SpriteComponent>(id) };
	SFMLSpriteComponent* sfmlSprite{ worldEntity.GetComponent<SFMLSpriteComponent>(id) };

	renderer.SubmitDrawSprite(&sfmlSprite->sprite, &sfmlSprite->renderStates, int(spriteComponent->GetZDepth()));
}
