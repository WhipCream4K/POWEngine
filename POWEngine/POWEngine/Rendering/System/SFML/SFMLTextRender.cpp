#include "pch.h"
#include "SFMLTextRender.h"

#include "POWEngine/Rendering/2D/Draw2DObject.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Text/SFML/SFMLTextComponent.h"

powe::SFMLTextRender::SFMLTextRender()
{
	using namespace powe;
	DEFINE_SYSTEM_KEY(Transform2D, SFMLTextComponent);
}

void powe::SFMLTextRender::OnDraw(const SFML2DRenderer& renderer, GameObjectID)
{
	const auto& [transform2D ,textComp] =
		GetComponentsView<Transform2D, SFMLTextComponent>();

	const glm::vec2 position{ transform2D->GetWorldPosition() };
	const glm::vec2 scale{ transform2D->GetWorldScale() };
	const float rotation{ transform2D->GetWorldRotation() };

	sf::RenderStates renderStates{};
	auto& sfTransform{ renderStates.transform };

	sfTransform.translate({ position.x,position.y });
	sfTransform.rotate(sf::degrees(rotation));
	sfTransform.scale({ scale.x,scale.y });

	renderer.SubmitDrawSprite(&textComp->text, renderStates, textComp->renderOrder);
}
