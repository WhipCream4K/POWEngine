#pragma once

#include "Observer.h"

/**
 * \brief Keep track of the signal from all of the plates in the scene
 */
class WinConditionListener : public Observer
{
public:

	WinConditionListener() = default;
	WinConditionListener(int maxServing);
	void OnReceiveMessage(powe::WorldEntity&, Subject*) override;

private:

	int m_ServeCount{};
	int m_MaxServingCount{};
};

