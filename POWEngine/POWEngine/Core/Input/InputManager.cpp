#include "pch.h"
#include "InputManager.h"

#include "Key.h"
#include "POWEngine/Window/WindowContext.h"
#include "POWEngine/Core/Input/InputStruct.h"
#include "POWEngine/Core/Input/InputVars.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#endif

class powe::InputManager::XInputImpl
{
public:

	enum class AxisType
	{
		Thumb,
		Shoulder
	};

	XInputImpl();
	void PollHardWareMessages(HardwareMessages& messages, float deltaTime);

private:

	bool ShouldWait(uint8_t playerIndex, float deltaTime);
	float ApplyDeadZone(AxisType axisType, int maxValue, int deadZone, int axisValue) const;

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
	for (float& time : m_RetryTimes)
	{
		time = m_DefaultRetryTime;
	}
}

void powe::InputManager::XInputImpl::PollHardWareMessages(HardwareMessages& messages, float deltaTime)
{
	int currentMessageCnt{ messages.totalMessages };

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
				GamepadData gamepadData{};

				gamepadData.buttons = state.Gamepad.wButtons;
				gamepadData.playerIndex = uint8_t(i);



				gamepadData.LShoulder = ApplyDeadZone(AxisType::Shoulder, UCHAR_MAX, gamepad::ShoulderDeadZone, int(state.Gamepad.bLeftTrigger));
				gamepadData.RShoulder = ApplyDeadZone(AxisType::Shoulder, UCHAR_MAX, gamepad::ShoulderDeadZone, int(state.Gamepad.bRightTrigger));

				gamepadData.thumbAxisData[GamepadKey::GetThumbIndexDataFromKeyCode(GamepadKey::GPK_Left_AxisX)] = 
					ApplyDeadZone(AxisType::Thumb, SHRT_MAX, gamepad::ThumbDeadZone, int(state.Gamepad.sThumbLX));

				gamepadData.thumbAxisData[GamepadKey::GetThumbIndexDataFromKeyCode(GamepadKey::GPK_Left_AxisY)] = 
					ApplyDeadZone(AxisType::Thumb, SHRT_MAX, gamepad::ThumbDeadZone, int(state.Gamepad.sThumbLY));

				gamepadData.thumbAxisData[GamepadKey::GetThumbIndexDataFromKeyCode(GamepadKey::GPK_Right_AxisX)] =
					ApplyDeadZone(AxisType::Thumb, SHRT_MAX, gamepad::ThumbDeadZone, int(state.Gamepad.sThumbRX));

				gamepadData.thumbAxisData[GamepadKey::GetThumbIndexDataFromKeyCode(GamepadKey::GPK_Right_AxisY)] =
					ApplyDeadZone(AxisType::Thumb, SHRT_MAX, gamepad::ThumbDeadZone, int(state.Gamepad.sThumbRY));

				//gamepadData.axisData.RThumbX = ApplyDeadZone(AxisType::Thumb, SHRT_MAX, gamepad::ThumbDeadZone, int(state.Gamepad.sThumbRX));
				//gamepadData.axisData.RThumbY = ApplyDeadZone(AxisType::Thumb, SHRT_MAX, gamepad::ThumbDeadZone, int(state.Gamepad.sThumbRY));



				auto& hwMessage{ messages.hwMessages[currentMessageCnt++] };
				hwMessage.hData = gamepadData;
				hwMessage.eventId = uint8_t(EventType::Gamepad);
				hwMessage.inDevice = InputDevice::D_Gamepad;

				m_ActivePlayers[i] = true;
				m_RetryTimes[i] = m_DefaultRetryTime;
			}
			else
			{
				m_ActivePlayers[i] = false;
				m_RetryTimes[i] += m_RetryInterval;
				if (m_RetryTimes[i] >= m_MaxRetryTime)
					m_RetryTimes[i] = m_MaxRetryTime;
			}
		}
	}

	messages.totalMessages = currentMessageCnt;
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

float powe::InputManager::XInputImpl::ApplyDeadZone(AxisType axisType, int maxValue, int deadZone, int axisValue) const
{
	float result{};
	switch (axisType)
	{
	case AxisType::Thumb:
	{
		const float sign{ axisValue < 0 ? -1.0f : 1.0f };
		axisValue = std::max(0, abs(axisValue) - deadZone);
		result = float(axisValue) * sign / float(maxValue - deadZone);
	}
	break;
	case AxisType::Shoulder:

		axisValue = std::max(0, axisValue - deadZone);
		result = float(axisValue) / float(maxValue - deadZone);

		break;
	default:;
	}

	return result;
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


