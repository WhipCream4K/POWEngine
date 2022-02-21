#include "pch.h"
#include "Core.h"

#include "Input/InputParams.h"
#include "Input/Key.h"
#include "POWEngine/Services/ServiceLocator.h"
#include "POWEngine/Window/Window.h"
#include "POWEngine/Core/Clock/WorldClock.h"
#include "POWEngine/Logger/LoggingService.h"
#include "WorldEntity/WorldEntity.h"

powe::Core::Core()
	: m_WorldClock()
{
	ServiceLocator::Initialize();
}

bool powe::Core::TranslateWindowInputs(const SharedPtr<Window>& window)
{
	m_WorldClock->Start();

	bool isEarlyExit{};
	bool ignoreInputs{};

	const WindowMessages messages{ window->PollWindowMessages(isEarlyExit,ignoreInputs) };

	// TODO: do the translation here

	return isEarlyExit;
}

bool powe::Core::TranslateWindowInputs(const Window& window, const SharedPtr<WorldEntity>& worldEntt) const
{
	m_WorldClock->Start();

	bool isEarlyExit{};
	bool ignoreInputs{};

	const WindowMessages& wndMessages{ window.PollWindowMessages(isEarlyExit,ignoreInputs) };

	if (!ignoreInputs)
	{
		// filter hardware input in this window

		std::array<WndMessageHWIdx, MinimumWindowEventCnt> hardWareInput{};
		uint8_t hardWareInCnt{};

		for (int i = 0; i < wndMessages.totalMessages; ++i)
		{
			// early exit when this window isn't focused
			const uint8_t eventId{ wndMessages.wndMessages[i].eventId };

			// TODO: Maybe do binary comparison

			// This is framework specific
			if (IsKeyboardEvent(eventId))
			{
				hardWareInput[int(hardWareInCnt++)] = 
					WndMessageHWIdx{uint8_t(i),InputDevice::D_Keyboard };
			}
			else if (IsMouseEvent(eventId))
			{
				hardWareInput[int(hardWareInCnt++)] =
					WndMessageHWIdx{ uint8_t(i), InputDevice::D_Mouse };
			}
		}

		worldEntt->GetInputSettings().ParseWndMessages(wndMessages, hardWareInput);
	}

	return isEarlyExit;
}

void powe::Core::StartWorldClock()
{
	m_WorldClock = std::make_shared<WorldClock>();
}

void powe::Core::Step(const SharedPtr<WorldEntity>&)
{
	// updates the world also create tasks for multi-thread rendering
}

powe::Core::~Core() = default;
