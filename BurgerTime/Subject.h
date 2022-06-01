#pragma once

#include <poweCustomtype.h>

namespace powe
{
	class WorldEntity;
}

class Observer;
class Subject
{
public:

	void Notify(powe::WorldEntity&);

	void Attach(Observer* observer);
	void Remove(Observer* observer);

	Subject() = default;
	virtual ~Subject() = default;

private:

	std::vector<Observer*> m_Observers;
};

