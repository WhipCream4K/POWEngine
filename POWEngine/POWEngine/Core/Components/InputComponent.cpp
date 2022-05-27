#include "pch.h"
#include "InputComponent.h"

powe::InputComponent::InputComponent(uint8_t playerIndex)
	: m_PlayerIndex(playerIndex)
{
}

void powe::InputComponent::AddActionCommand(const std::string& name, 
	InputEvent inputEvent,
	const SharedPtr<ActionCommand>& actionCommand)
{
	auto& commands{ m_ActionCommands[name] };
	if (std::ranges::find_if(commands, [&inputEvent,&actionCommand](const ActionPack& actionPack)
	{
			return inputEvent == actionPack.targetEvent && actionCommand == actionPack.command;
	}) == commands.end())
	{
		commands.emplace_back(ActionPack{actionCommand,inputEvent});
	}
}

void powe::InputComponent::AddAxisCommand(const std::string& name, const SharedPtr<AxisCommand>& axisCommand)
{
	auto& commands{ m_AxisCommands[name] };
	if (std::ranges::find(commands, axisCommand) == commands.end())
	{
		commands.emplace_back(axisCommand);
	}
}
