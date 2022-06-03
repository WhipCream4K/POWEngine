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

	if(GetWorld()->GetInputSettings().GetInputAction("ShowDebug",InputEvent::IE_Pressed))
	{
		const auto& debugRect{ GetComponent<DebugRectangle>() };
		debugRect->SetShouldRender(!debugRect->GetShouldRender());
	}
}
