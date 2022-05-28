#include "pch.h"
#include "InputManager.h"

#include "POWEngine/Window/WindowContext.h"
#include "POWEngine/Core/Input/InputStruct.h"
#include "POWEngine/Core/Input/InputVars.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#endif

class powe::InputManager::XInputImpl
{
public:

	XInputImpl();
	void PollHardWareMessages(HardwareMessages& hwMessages, float deltaTime);

private:

	bool ShouldWait(uint8_t playerIndex, float deltaTime);

	std::array<bool, MAXPLAYER> m_ActivePlayers{};
	std::array<float, MAXPLAYER> m_WaitTimes{};
	//float m_RetryTime{ 1.5f }; // will try to get xinput state once every 1.5 seconds
	std::array<float, MAXPLAYER> m_RetryTimes{};
	float m_DefaultRetryTime{ 1.5f };
	float m_MaxRetryTime{ 5.0f };
	float m_RetryInterval{ 0.5f };
};

powe::InputManager::XInputImpl::XInputImpl()
{
	for (float& time: m_RetryTimes)
	{
		time = m_DefaultRetryTime;
	}
}

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

				// TODO: Parse axis input and manage deadzone
				hwMessages.hwMessages[currentMessageCnt++].hData = GamepadButtonData{ state.Gamepad.wButtons,uint8_t(i) };
				m_ActivePlayers[i] = true;
				m_RetryTimes[uint8_t(i)] = m_DefaultRetryTime;
			}
			else
			{
				m_ActivePlayers[i] = false;
				m_RetryTimes[uint32_t(i)] += m_RetryInterval;
				if (m_RetryTimes[uint32_t(i)] >= m_MaxRetryTime)
					m_RetryTimes[uint8_t(i)] = m_MaxRetryTime;
			}
		}
	}
}

bool powe::InputManager::XInputImpl::ShouldWait(uint8_t playerIndex, float deltaTime)
{
	if (m_ActivePlayers[playerIndex])
		return false;

	if (m_WaitTimes[playerIndex] < m_RetryTimes[playerIndex])
	{
		m_WaitTimes[playerIndex] += deltaTime;
		return true;
	}

	m_WaitTimes[playerIndex] -= m_RetryTimes[playerIndex];
	return false;
}

powe::InputManager::InputManager()
	: m_XInputImpl(std::make_unique<XInputImpl>())
{
}

void powe::InputManager::PollHardWareMessages(HardwareMessages& hwMessages, float deltaTime) const
{
	m_XInputImpl->PollHardWareMessages(hwMessages, deltaTime);
}

powe::InputManager::~InputManager() = default;


