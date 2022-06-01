#include "Subject.h"

#include "Observer.h"
#include <algorithm>

void Subject::Notify(powe::WorldEntity& worldEntity)
{
	for (auto it = m_Observers.begin(); it != m_Observers.end();)
	{
		if (const auto observer{ *it })
		{
			observer->OnReceiveMessage(worldEntity, this);
			++it;
		}
		else
			it = m_Observers.erase(it);
	}
}

void Subject::Attach(Observer* observer)
{
	if (std::ranges::find(m_Observers, observer) == m_Observers.end())
	{
		m_Observers.emplace_back(observer);
	}
}

void Subject::Remove(Observer* observer)
{
	const auto removeItr{ std::ranges::find(m_Observers,observer) };

	if (removeItr != m_Observers.end())
		m_Observers.erase(removeItr);
}
