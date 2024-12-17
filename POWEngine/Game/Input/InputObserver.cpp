#include "pch.h"
#include "InputObserver.h"
#include "Game/Game.h"

using namespace powe;

InputObserver::~InputObserver()
{
    auto gameModule{ Application::GetModule<Game>() };
    if(!gameModule)
        return;

    gameModule->GetInputManager().RemoveObserver(shared_from_this());
}