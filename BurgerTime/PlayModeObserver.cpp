#include "PlayModeObserver.h"

#include "BurgerTimeComponents.h"
#include "PlayModeSubject.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

PlayModeObserver::PlayModeObserver(powe::GameObjectID owner)
	: m_Owner(owner)
{
}

void PlayModeObserver::OnReceiveMessage(powe::WorldEntity& worldEntity, Subject* subject)
{
	if (const auto playModeSubject{ dynamic_cast<PlayModeSubject*>(subject) })
	{
		if (DynamicSceneData * data{ worldEntity.GetComponent<DynamicSceneData>(m_Owner) })
		{
			data->currentPlayMode = playModeSubject->PlayMode;
		}
	}
}
