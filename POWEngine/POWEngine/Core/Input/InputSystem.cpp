#include "pch.h"
#include "InputSystem.h"

#include "POWEngine/Core/Components/InputComponent.h"
#include "POWEngine/Core/Input/InputStruct.h"

powe::InputSystem::InputSystem()
{
	// DEFINE_SYSTEM_KEY(InputComponent);
	DefineSystemKeys<InputComponent>();
}

void powe::InputSystem::OnUpdate(float deltaTime, powe::GameObjectID id)
{
	const InputComponent* inputComponent{ GetComponent<InputComponent>() };
	const InputSettings& inputSettings{ GetWorld()->GetInputSettings() };

	const auto& realGamePadKey{ inputSettings.GetCurrentKeyState(inputComponent->GetRealPlayerIndex()) };

	const auto& worldAxisMap{ inputSettings.GetAxisMap() };

	for (const auto& [axisName, commands] : inputComponent->GetAxisCommands())
	{
		if (commands.empty())
			continue;

		ExecuteAxisCommands(worldAxisMap, axisName, realGamePadKey, commands, deltaTime, id);
	}

	const auto& worldActionMap{ inputSettings.GetActionMap() };
	const SysKeyType thisFrameSysKey{ inputSettings.GetThisFrameSysKey() };

	for (const auto& [actionName, actionPack] : inputComponent->GetActionCommands())
	{
		if (actionPack.empty())
			continue;

		ExecuteActionCommands(worldActionMap, actionName, realGamePadKey, actionPack, thisFrameSysKey, deltaTime, id);
	}

}


void powe::InputSystem::ExecuteAxisCommands(
	const std::unordered_map<std::string, AxisMap>& worldAxisMap,
	const std::string& actionName,
	const InputSettings::KeyPool& priorityKeyPool,
	const std::vector<SharedPtr<AxisCommand>>& commands,
	float deltaTime,
	GameObjectID id) const
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
					command->Execute(*GetWorld(), deltaTime, id, currentState.axisThisFrame * scale);
				}
			}
		}
	}
}

void powe::InputSystem::ExecuteActionCommands(
	const std::unordered_map<std::string, ActionMap>& worldActionMap,
	const std::string& actionName,
	const InputSettings::KeyPool& priorityKeyPool,
	const std::vector<SharedPtr<ActionCommand>>& commands,
	SysKeyType inSysKey,
	float deltaTime,
	GameObjectID id) const
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
				for (const auto& command : commands)
				{
					command->Execute(*GetWorld(), deltaTime, id,thisFrameEvent);
				}
			}
		}
	}
}

