#include "PepperThrowListener.h"

#include "OnPlayerThrowPepper.h"

void PepperThrowListener::OnReceiveMessage(powe::WorldEntity& worldEntity, Subject* subject)
{
	if(const auto s = dynamic_cast<OnPlayerThrowPepper*>(subject))
	{
		OnReceiveThrowPepper(worldEntity);
	}
}
