#include "pch.h"
#include "InputSystem.h"

#include "POWEngine/Core/Components/InputComponent.h"

powe::InputSystem::InputSystem()
{
	DEFINE_SYSTEM_KEY(InputComponent);
}

void powe::InputSystem::OnUpdate(float, powe::GameObjectID)
{
	const InputComponent* inputComponent{ GetComponent<InputComponent>() };
	const InputSettings& inputSettings{ GetWorld()->GetInputSettings() };

	const auto& realGamePadKey{ inputSettings.GetCurrentKeyState(inputComponent->GetRealPlayerIndex()) };

	const auto& worldAxisMap{ inputSettings.GetAxisMap() };

	for (const auto& [axisName, commands] : inputComponent->GetAxisCommands())
	{
		if (commands.empty())
			continue;

		ExecuteAxisCommands(worldAxisMap, axisName, realGamePadKey, commands);
	}

	const auto& worldActionMap{ inputSettings.GetActionMap() };
	const SysKeyType thisFrameSysKey{ inputSettings.GetThisFrameSysKey() };

	for (const auto& [actionName, actionPack] : inputComponent->GetActionCommands())
	{
		if (actionPack.empty())
			continue;

		ExecuteActionCommands(worldActionMap, actionName, realGamePadKey, actionPack, thisFrameSysKey);
	}

}


void powe::InputSystem::ExecuteAxisCommands(
	const std::unordered_map<std::string, AxisMap>& worldAxisMap,
	const std::string& actionName,
	const InputSettings::KeyPool& priorityKeyPool,
	const std::vector<SharedPtr<AxisCommand>>& commands) const
{
	const auto findItr{ worldAxisMap.find(actionName) };

	if (findItr == worldAxisMap.end())
		return;


	for (const auto& [key, scale] : findItr->second.keyPool)
	{
		if (priorityKeyPool.contains(key))
		{
			const auto& currentState{ priorityKeyPool.at(key) };

			if (!currentState.inputThisFrame && !currentState.inputLastFrame)
				continue;

			if (bool(currentState.axisThisFrame))
			{
				for (const auto& command : commands)
				{
					command->Execute(*GetWorld(), currentState.axisThisFrame * scale);
				}
			}
		}
	}
}

void powe::InputSystem::ExecuteActionCommands(
	const std::unordered_map<std::string, ActionMap>& worldActionMap,
	const std::string& actionName,
	const InputSettings::KeyPool& priorityKeyPool,
	const std::vector<ActionPack>& commands,
	SysKeyType inSysKey) const
{
	const auto findItr{ worldActionMap.find(actionName) };

	if (findItr == worldActionMap.end())
		return;

	for (const auto& key : findItr->second.keyPool)
	{
		if (priorityKeyPool.contains(key))
		{
			const auto& currentState{ priorityKeyPool.at(key) };

			if (!currentState.inputThisFrame && !currentState.inputLastFrame)
				continue;

			const InputEvent thisFrameEvent{ (currentState.inputLastFrame << 0) | (currentState.inputThisFrame << 1) };

			// Check for sysKey
			SysKeyType sysCouple{};
			const auto sysKeyItr{ findItr->second.systemKeyHandle.find(key) };
			if (sysKeyItr != findItr->second.systemKeyHandle.end())
				sysCouple = sysKeyItr->second;

			if (thisFrameEvent != InputEvent::IE_None && (sysCouple == 0 || (inSysKey & sysCouple)))
			{
				for (const auto& [command, targetEvent] : commands)
				{
					if (targetEvent == thisFrameEvent)
						command->Execute(*GetWorld());
				}
			}
		}
	}
}

