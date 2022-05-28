#include "pch.h"
#include "InputManager.h"

#include "POWEngine/Window/WindowContext.h"
#include "POWEngine/Core/Input/InputStruct.h"
#include "POWEngine/Core/Input/InputVars.h"

#ifdef WIN32
#include <Xinput.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

class powe::InputManager::XInputImpl
{
public:

	void PollHardWareMessages(HardwareMessages& hwMessages, float deltaTime);

private:

	bool ShouldWait(uint8_t playerIndex, float deltaTime);

	std::array<bool, MAXPLAYER> m_ActivePlayer{};
	std::array<float, MAXPLAYER> m_WaitTime{};
	float m_RetryTime{ 1.5f }; // will try to get xinput state once every 1.5 seconds
};

void powe::InputManager::XInputImpl::PollHardWareMessages(HardwareMessages& hwMessages, float deltaTime)
{
	int currentMessageCnt{ hwMessages.totalMessages };

	DWORD dwResult{};
	for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		if (!ShouldWait(uint8_t(i), deltaTime))
		{
			XINPUT_STATE state{};
			ZeroMemory(&state, sizeof state);

			// highly taxing the performance when there's no controller connected
			dwResult = XInputGetState(i, &state);

			if (dwResult == ERROR_SUCCESS)
			{
				// controller is connected
				hwMessages.hwMessages[currentMessageCnt++].hData = GamepadButtonData{ state.Gamepad.wButtons,uint8_t(i) };
				m_ActivePlayer[i] = true;
			}
			else
			{
				m_ActivePlayer[i] = false;
			}
		}
	}
}

bool powe::InputManager::XInputImpl::ShouldWait(uint8_t playerIndex, float deltaTime)
{
	if (m_ActivePlayer[playerIndex])
		return false;

	if (m_WaitTime[playerIndex] < m_RetryTime)
	{
		m_WaitTime[playerIndex] += deltaTime;
		return true;
	}

	m_WaitTime[playerIndex] = 0.0f;
	return false;
}

powe::InputManager::InputManager() = default;

void powe::InputManager::PollHardWareMessages(HardwareMessages& hwMessages, float deltaTime) const
{
	m_XInputImpl->PollHardWareMessages(hwMessages, deltaTime);
}

powe::InputManager::~InputManager() = default;


