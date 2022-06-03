#include "DebugControllerSystem.h"

#include "POWEngine/Rendering/Components/Debug/DebugRectangle.h"

DebugControllerSystem::DebugControllerSystem()
{
	using namespace powe;

	DEFINE_SYSTEM_KEY(DebugRectangle);
}

void DebugControllerSystem::OnUpdate(float, powe::GameObjectID)
{
	using namespace powe;

	if(GetWorld()->GetInputSettings().GetInputAxis("ShowDebug") > 0.0f)
	{
		const auto& debugRect{ GetComponent<DebugRectangle>() };
		debugRect->SetShouldRender(!debugRect->GetShouldRender());
	}
}
