#include "MenuCommands.h"

#include "BurgerTimeComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"


void MenuCommands::Execute(powe::WorldEntity& worldEntity, float , powe::GameObjectID id, float axisValue)
{
	using namespace powe;

	MenuData* menuData{ worldEntity.GetComponent<MenuData>(id) };
	powe::Transform2D* transform2D{ worldEntity.GetComponent<Transform2D>(id) };
	if(menuData && transform2D)
	{
		const int sign{ axisValue < 0.0f ? -1 : 1 };
		//menuData->m_OptionPos1 = {};
		int selectedPlayIndex{ int(menuData->playModeSubject->PlayMode) };
		selectedPlayIndex = std::max(0,selectedPlayIndex + sign);
		menuData->playModeSubject->PlayMode = PlayMode(selectedPlayIndex % int(PlayMode::Count));
		menuData->playModeSubject->Notify(worldEntity);

		transform2D->SetWorldPosition(burger::MenuPos[selectedPlayIndex]);
	}
}
