#pragma once

#include "Observer.h"

/**
 * \brief Keep track of the signal from all of the plates in the scene
 */
class WinConditionListener : public Observer
{
public:

	WinConditionListener() = default;
	WinConditionListener(powe::GameObjectID owner,powe::GameObjectID audioManager,int maxServing);
	void OnReceiveMessage(powe::WorldEntity&, BurgerEvent) override;
	//void OnReceiveMessage(powe::WorldEntity&, Subject*) override;
	void SetMaxServingCount(int servingCount);
	void Reset();

private:

	powe::GameObjectID m_Owner{};
	powe::GameObjectID m_AudioManager{};
	int m_ServeCount{};
	int m_MaxServingCount{};
};

