#pragma once

#include <poweCustomtype.h>


namespace powe
{
	class WorldEntity;
}

class Subject;
class Observer
{
public:

	virtual void OnReceiveMessage(powe::WorldEntity&,Subject*) = 0;

	Observer() = default;
	virtual ~Observer() = default;
};

