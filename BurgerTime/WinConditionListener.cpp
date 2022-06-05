#include "WinConditionListener.h"

#include "OnIngredientServing.h"

WinConditionListener::WinConditionListener(int maxServing)
	: m_MaxServingCount(maxServing)
{
}

void WinConditionListener::OnReceiveMessage(powe::WorldEntity&, Subject* subject)
{
	if(const auto serving{dynamic_cast<OnIngredientServing*>(subject)})
	{
		serving;
		++m_ServeCount;
		if(m_ServeCount >= m_MaxServingCount)
		{
			// Trigger game end
		}
	}
}
