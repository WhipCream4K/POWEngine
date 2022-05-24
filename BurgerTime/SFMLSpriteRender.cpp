#include "SFMLSpriteRender.h"

#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"


SFMLSpriteRender::SFMLSpriteRender()
{
	using namespace powe;
	DEFINE_SYSTEM_KEY(SpriteComponent);
}

void SFMLSpriteRender::OnDraw(
	const powe::WorldEntity& worldEntity, 
	const powe::SFML2DRenderer& renderer,
	powe::GameObjectID id)
{
	using namespace powe;

	SpriteComponent* sprite{ worldEntity.GetComponent<SpriteComponent>(id) };
}
