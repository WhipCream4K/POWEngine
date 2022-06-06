#include "ScoreListener.h"

#include "BurgerTimeComponents.h"
#include "OnIngredientDropToPlatform.h"
#include "ScorableEvent.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Rendering/Components/Text/TextComponent.h"

ScoreListener::ScoreListener(powe::GameObjectID displayManager)
	: m_DisplayManager(displayManager)
{
}

void ScoreListener::OnReceiveMessage(powe::WorldEntity&, BurgerEvent)
{

}

void ScoreListener::OnReceiveMessageByPlayer(powe::WorldEntity& worldEntity, BurgerEvent burgerEvent, int playerIndex)
{
	using namespace powe;

	DisplayManager* displayManager = worldEntity.GetComponent<DisplayManager>(m_DisplayManager);
	if (displayManager)
	{

		if (playerIndex > int(PlayMode::Count))
			return;

		int score{};
		switch (burgerEvent)
		{
		//case BurgerEvent::EnemySquash:				score = 50; break;
		case BurgerEvent::HotDogDead:				score = 100;	break;
		case BurgerEvent::IngredientDropToPlatform:	score = 50;				break;
		default: ;
		}

		if (const auto& scoreObject = displayManager->score[playerIndex])
		{
			TextComponent* textComponent = scoreObject->GetComponent<TextComponent>();

			if (!textComponent)
				return;

			displayManager->CurrentScore[playerIndex] += score;
			textComponent->SetText(std::to_string(displayManager->CurrentScore[playerIndex]));
		}

	}

}

