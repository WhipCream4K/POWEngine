#include "HUDDisplay.h"
#include "BurgerTimeComponents.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"

HUDDisplay::HUDDisplay(powe::GameObjectID displayManager)
	: m_DisplayManager(displayManager)
{
}

void HUDDisplay::OnReceiveMessageByPlayer(powe::WorldEntity& worldEntity, BurgerEvent burgerEvent, int playerIndex)
{
	DisplayManager* displayManager{ worldEntity.GetComponent<DisplayManager>(m_DisplayManager) };
	if(displayManager)
	{
		switch (burgerEvent)
		{
		case BurgerEvent::PlayerDead:
			HandlePlayerHealth(worldEntity, displayManager, playerIndex);
			break;
		}
	}
}

void HUDDisplay::OnReceiveMessage(powe::WorldEntity&, BurgerEvent)
{
}

void HUDDisplay::HandlePlayerHealth(powe::WorldEntity&, DisplayManager* displayManager, int playerIndex) const
{
	using namespace powe;

	if(playerIndex > int(PlayMode::Count) && playerIndex < 0)
		return;

	if (displayManager->CurrentLives[playerIndex] < 1)
		return;

	--displayManager->CurrentLives[playerIndex];
	const int currentLives{ displayManager->CurrentLives[playerIndex] };

	const auto& healthObj{ displayManager->healthDisplay[playerIndex] };
	if(healthObj)
	{
		SpriteComponent* spriteComponent = healthObj->GetComponent<SpriteComponent>();
		if (!spriteComponent)
			return;

		const float spriteWidth{ 8.0f };
		spriteComponent->SetRect({ 0.0f,0.0f,spriteWidth,spriteWidth * float(currentLives) });
	}

}

