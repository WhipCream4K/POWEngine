#include "PepperNumberDisplay.h"

#include "BurgerTimeComponents.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Rendering/Components/Text/TextComponent.h"

PepperNumberDisplay::PepperNumberDisplay(powe::GameObjectID displayManager)
	: m_DisplayManager(displayManager)
{
}

void PepperNumberDisplay::OnReceiveMessageByPlayer(powe::WorldEntity& worldEntity, BurgerEvent, int playerIndex)
{
	using namespace powe;

	DisplayManager* displayManager{ worldEntity.GetComponent<DisplayManager>(m_DisplayManager) };
	if (displayManager)
	{

		if (displayManager->CurrentPepper[playerIndex] < 0)
			return;

		--displayManager->CurrentPepper[playerIndex];

		const auto& pepperNumObj = displayManager->pepperNumber[playerIndex];
		if (pepperNumObj)
		{
			powe::TextComponent* textComponent = pepperNumObj->GetComponent<TextComponent>();
			if (!textComponent)
				return;

			const std::string pepNum{ std::to_string(displayManager->CurrentPepper[playerIndex]) };
			textComponent->SetText(pepNum);
		}
	}

}

void PepperNumberDisplay::OnReceiveMessage(powe::WorldEntity&, BurgerEvent)
{

}

