#include "PlayScene.h"

#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

PlayScene::PlayScene()
{
}

void PlayScene::Load(powe::WorldEntity& )
{
	
}

void PlayScene::UnLoad(powe::WorldEntity& worldEntity)
{
	for (const auto& gameObject : m_GameObjects)
	{
		worldEntity.RemoveGameObject(gameObject->GetID());
	}

	m_GameObjects.clear();
}
