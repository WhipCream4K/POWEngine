#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Core/Input/ListsOfKeys.h"


namespace powe
{

	class ActionCommand
	{
	public:

		ActionCommand() = default;
		virtual ~ActionCommand() = default;

		virtual void Execute(WorldEntity& worldEntity, float deltaTime, GameObjectID id) = 0;
	};

	class AxisCommand
	{
	public:

		AxisCommand() = default;
		virtual ~AxisCommand() = default;

		virtual void Execute(WorldEntity& worldEntity, float deltaTime, GameObjectID id, float axisValue) = 0;
	};

	struct ActionPack
	{
		SharedPtr<ActionCommand> command{};
		InputEvent targetEvent{};
	};

	class GameObject;
	class InputComponent : public Component<InputComponent>
	{


	public:

		InputComponent(uint8_t playerIndex = 0);

		void AddActionCommand(const std::string& name, InputEvent inputEvent, const SharedPtr<ActionCommand>& actionCommand);
		void AddAxisCommand(const std::string& name, const SharedPtr<AxisCommand>& axisCommand);

		uint8_t GetRealPlayerIndex() const { return m_PlayerIndex; }



		const std::unordered_map<std::string, std::vector<ActionPack>> GetActionCommands() const { return m_ActionCommands; }
		const std::unordered_map<std::string, std::vector<SharedPtr<AxisCommand>>> GetAxisCommands() const { return m_AxisCommands; }

	private:

		std::unordered_map<std::string, std::vector<ActionPack>> m_ActionCommands{};
		std::unordered_map<std::string, std::vector<SharedPtr<AxisCommand>>> m_AxisCommands{};
		uint8_t m_PlayerIndex{};
	};
}



