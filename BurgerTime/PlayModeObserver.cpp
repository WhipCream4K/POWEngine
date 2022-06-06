#include "PlayModeObserver.h"

#include "BurgerTimeComponents.h"
#include "PlayModeSubject.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

PlayModeObserver::PlayModeObserver(powe::GameObjectID owner)
	: m_Owner(owner)
{
}

void PlayModeObserver::OnReceiveMessage(powe::WorldEntity& worldEntity, BurgerEvent)
{
	if (DynamicSceneData * data{ worldEntity.GetComponent<DynamicSceneData>(m_Owner) })
	{
		int mode = int(data->currentPlayMode) + 1 % int(PlayMode::Count);
		data->currentPlayMode = PlayMode(mode);
	}
}
