#pragma once

#include "POWEngine/ECS/SystemBase.h"

namespace powe
{
	class AxisCommand;
	class ActionCommand;
	struct ActionPack;
	class InputSystem : public SystemBase
	{
	public:

		InputSystem();

	protected:

		void OnUpdate(float, powe::GameObjectID) override;

	private:

		void ExecuteAxisCommands(
			const std::unordered_map<std::string, AxisMap>& worldAxisMap,
			const std::string& actionName,
			const InputSettings::KeyPool& priorityKeyPool,
			const std::vector<SharedPtr<AxisCommand>>& commands) const;

		void ExecuteActionCommands(
			const std::unordered_map<std::string, ActionMap>& worldActionMap,
			const std::string& actionName,
			const InputSettings::KeyPool& priorityKeyPool,
			const std::vector<ActionPack>& commands,
			SysKeyType inSysKey) const;
	};
}


