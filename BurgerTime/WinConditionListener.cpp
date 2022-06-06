#include "WinConditionListener.h"

#include "BurgerTimeComponents.h"
#include "OnIngredientServing.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "AudioManager.h"
#include "POWEngine/Sound/SoundInfo.h"
#include <ranges>

WinConditionListener::WinConditionListener(powe::GameObjectID owner, powe::GameObjectID audioManager, int maxServing)
	: m_Owner(owner)
	, m_AudioManager(audioManager)
	, m_MaxServingCount(maxServing)
{}

void WinConditionListener::OnReceiveMessage(powe::WorldEntity& worldEntity, BurgerEvent)
{
	++m_ServeCount;
	if (m_ServeCount >= m_MaxServingCount)
	{
		// Trigger game end
		DynamicSceneData* sceneData = worldEntity.GetComponent<DynamicSceneData>(m_Owner);

		// Disable input
		if (sceneData)
		{
			sceneData->intervalPlayState = IntervalPlayState::LevelWin;

			for (const auto& systems : sceneData->blockingSystem | std::views::values)
			{
				for (const auto& systemBase : systems)
				{
					worldEntity.RemoveSystem(systemBase);
				}
			}
		}

		// Can only play the sound here
		AudioManager* audioManager = worldEntity.GetComponent<AudioManager>(m_AudioManager);
		if (audioManager)
		{
			powe::SoundInfo info{};
			info.pitch = 1.0f;
			info.volume = 0.5f;
			audioManager->PlaySound(BasicSound::GameWin, info);
		}
	}
}

//void WinConditionListener::OnReceiveMessage(powe::WorldEntity& worldEntity, Subject* subject)
//{
//	if (subject)
//	{
//
//	}
//}

void WinConditionListener::SetMaxServingCount(int servingCount)
{
	m_MaxServingCount = servingCount;
}

void WinConditionListener::Reset()
{
	m_ServeCount = 0;
}
