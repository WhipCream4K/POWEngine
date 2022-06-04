#include "SubIngredientStepHandler.h"

#include "SceneFactory.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Core/GameObject/GameObject.h"

SubIngredientStepHandler::SubIngredientStepHandler(const SharedPtr<powe::GameObject>& owner, int)
	: m_Owner(owner)
	, m_StepCount()
	, m_MaxStepCount()
{
}

void SubIngredientStepHandler::IncreaseStep()
{
	using namespace powe;
	++m_StepCount;

	if (m_StepCount >= m_MaxStepCount)
	{
		if (const auto owner{ m_Owner.lock() })
		{
			Transform2D* transform2D = owner->GetComponent<Transform2D>();
			if (!transform2D)
				return;

			for (const auto& child : transform2D->GetChildren())
			{
				if (const auto childObject{ child.lock() })
				{
					childObject->GetWorld().RemoveGameObject(childObject->GetID());

					if(const auto scene{m_Scene.lock()})
					{
						scene->RemoveGameObject(childObject);
					}

				}
			}



		}
	}
}
