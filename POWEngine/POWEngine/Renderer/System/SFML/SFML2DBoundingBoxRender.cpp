#include "pch.h"
#include "SFML2DBoundingBoxRender.h"

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Renderer/Components/Debug2D/DebugRectangle.h"
#include "POWEngine/Renderer/Components/Debug2D/SFML/SFMLDebugRectangleComponent.h"

powe::SFML2DBoundingBoxRender::SFML2DBoundingBoxRender()
{
	//DEFINE_SYSTEM_KEY(Transform2D, DebugRectangle, SFMLDebugRectangleComponent);
	DefineSystemKeys<Transform2D, DebugRectangle, SFMLDebugRectangleComponent>();
}

//
// void powe::SFML2DBoundingBoxRender::OnDraw(const SFML2DRenderer& renderer, GameObjectID)
// {
// 	const auto& debugRect{ GetComponent<DebugRectangle>() };
// 	if (debugRect->GetShouldRender())
// 	{
// 		const auto& [transform2D, sfmlDraw] =
// 			GetComponentsView<Transform2D, SFMLDebugRectangleComponent>();
//
// 		const glm::vec2 position{ transform2D->GetPosition() };
// 		//const glm::vec2 scale{ transform2D->GetWorldScale() };
// 		const float rotation{ transform2D->GetRotation() };
//
// 		sf::RenderStates renderStates{};
// 		auto& sfTransform{ renderStates.transform };
//
// 		sfTransform.translate({ position.x,position.y });
// 		sfTransform.rotate(glm::degrees(rotation));
//
// 		//sfTransform.scale({ scale.x,scale.y });
//
// 		renderer.SubmitDrawSprite(&sfmlDraw->rectangle, renderStates, sfmlDraw->drawOrder);
// 	}
//
// }
