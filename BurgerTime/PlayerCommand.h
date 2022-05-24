#pragma once

namespace powe
{
	class WorldEntity;
}

class PlayerCommand
{
public:

	virtual void Execute(const powe::WorldEntity&) = 0;
	virtual ~PlayerCommand() = default;
};

