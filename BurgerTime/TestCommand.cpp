#include "TestCommand.h"

#include "POWEngine/Logger/LoggerUtils.h"

void TestCommand::Execute(powe::WorldEntity&, float axisValue)
{
	const std::string ey{ std::to_string(axisValue) };
	POWLOGNORMAL(ey);
}

void FireCommand::Execute(powe::WorldEntity&)
{
	POWLOGNORMAL("I'm shooting");
}
