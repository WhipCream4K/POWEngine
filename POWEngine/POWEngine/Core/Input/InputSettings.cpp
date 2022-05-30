#include "pch.h"
#include "InputSettings.h"

#include "InputStruct.h"
#include "POWEngine/Logger/LoggerUtils.h"

#if USE_SFML_WINDOW

#include <SFML/Window.hpp>

//void powe::InputSettings::ParseWndMessages(
//	const WindowMessages& winMessages,
//	const std::array<WndMessageHWIdx, MinimumWindowEventCnt>& inputHardWareIdx)
//{
//
//	Key itrKey{};
//
//	// KeyPool lookUp vs iterate KeyPool
//	for (const auto& input : inputHardWareIdx)
//	{
//		// TODO: Maybe take care of out of range
//
//		// maybe overflow operation uint8_t -> int
//		const MessageBus msgBus{ winMessages.wndMessages[int(input.idx)] };
//
//		switch (input.hardWare)
//		{
//		case InputDevice::D_Keyboard:
//		{
//			itrKey.inputDevice = InputDevice::D_Keyboard;
//
//			// TODO: Take care of syskey
//			const auto& sfKey{ std::any_cast<sf::Event::KeyEvent>(msgBus.data) };
//			itrKey.keyCode = uint16_t(sfKey.code);
//
//			ParseKeyboardKey(msgBus.eventId, itrKey);
//
//			break;
//		}
//		case InputDevice::D_Mouse:
//		{
//			try
//			{
//				itrKey.inputDevice = InputDevice::D_Mouse;
//
//				const auto& sfMouse{ std::any_cast<sf::Event::MouseButtonEvent>(msgBus.data) };
//				itrKey.keyCode = uint16_t(sfMouse.button);
//
//				ParseMouseKey(msgBus.eventId, itrKey);
//			}
//			catch (const std::exception& some)
//			{
//				//POWLOGERROR(some.what());
//				POWLOGINFO(some.what());
//			}
//
//			break;
//		}
//		case InputDevice::D_Gamepad:
//
//			//TODO: Support the gamepad
//
//
//			break;
//		default:;
//		}
//	}
//
//	//for (const auto& keyPool : m_MainKeyPool)
//	//{
//	//	switch (keyPool.first.inputDevice)
//	//	{
//	//	case InputDevice::D_Keyboard:
//	//	{
//	//		for (const auto& input : inputHardWareIdx)
//	//		{
//	//			if(InputDevice(input.hardWare) == InputDevice::D_Keyboard)
//	//			{
//	//				
//	//			}
//	//		}
//	//	}
//
//	//	case InputDevice::D_Mouse: break;
//	//	case InputDevice::D_Gamepad: break;
//	//	default:;
//	//	}
//	//}
//}

void powe::InputSettings::ParseHWMessages(const HardwareMessages& hwMessages)
{
	// we need syskeys alive through this frame
	// to check against all the key that needs them
	//uint8_t sysKeys{};

	m_LastFrameSystemKey = m_CurrentFrameSystemKey;

	UpdateMainKeyPool();

	for (int i = 0; i < hwMessages.totalMessages; ++i)
	{
		const HardwareBus& hwBus{ hwMessages.hwMessages[i] };
		ProcessHWData(hwBus);

		//switch (hwBus.inDevice)
		//{
		//case InputDevice::D_Keyboard: 

		//	break;
		//case InputDevice::D_Mouse: break;
		//case InputDevice::D_Gamepad: break;
		//default: ;
		//}

		//KeyData inKey{};

		//if (hwBus.inDevice == InputDevice::D_Keyboard)
		//{
		//	//const bool isKeyPressed{ WindowEvents(hwBus.eventId) == WindowEvents::KeyPressed ? true : false };
		//	KeyboardData kData{ std::get<KeyboardData>(hwBus.hData) };
		//	inKey.isDown = kData.isPressed;
		//	inKey.key = { hwBus.inDevice,kData.keyCode };
		//	inKey.axisValue = float(kData.isPressed);
		//	inKey.playerIndex = 0;
		//	sysKeys |= kData.sysKey; // syskeys here
		//}
		//else if (hwBus.inDevice == InputDevice::D_Mouse)
		//{
		//	inKey.playerIndex = 0;

		//	//switch (WindowEvents(hwBus.eventId))
		//	//{
		//	//case WindowEvents::MouseWheelScrolled:
		//	//{
		//	//	MouseData mouseData{ std::get<MouseData>(hwBus.hData) };
		//	//	inKey.key = { hwBus.inDevice,MouseKey::MK_Middle };
		//	//	inKey.isDown = true;
		//	//	inKey.axisValue = std::get<MouseWheelDelta>(mouseData.axisData);
		//	//}
		//	//case WindowEvents::MouseButtonPressed:
		//	//{
		//	//	MouseData mData{ std::get<MouseData>(hwBus.hData) };
		//	//	inKey.key = { hwBus.inDevice,mData.keyCode };
		//	//	inKey.isDown = true;
		//	//}
		//	//case WindowEvents::MouseButtonReleased:
		//	//{
		//	//	MouseData mData{ std::get<MouseData>(hwBus.hData) };
		//	//	inKey.key = { hwBus.inDevice,mData.keyCode };
		//	//	inKey.isDown = false;
		//	//}
		//	//default: break;
		//	//}

		//	// If we past this line then we know that this is a mouse move
		//	//ValidateMouseDelta(GetMouseData<MousePos>(hwBus.hData));
		//	//m_ShouldRevalidateMouseValue = true;
		//}

		//m_CurrentFrameSystemKey |= sysKeys;
		//const InputEvent thisFrameEvent{ EvaluateMainKeyPool(inKey, sysKeys) };
		//UpdateAxisMapping(inKey, thisFrameEvent);
	}


}

float powe::InputSettings::GetInputAxis(const std::string& axisName, uint8_t playerIndex) const
{
	//try
	//{
	//}
	//catch (const std::exception& e)
	//{
	//	std::string log{ e.what() };
	//	log.append(" Can't find given axis name, maybe you forgot to add it?");
	//	POWLOGERROR(log);
	//}

	assert(playerIndex < MAXPLAYER && "Player index isn't not in range of max player");
	const auto findItr{ m_AxisKeyMappings.find(axisName) };
	if (findItr != m_AxisKeyMappings.end())
	{
		return findItr->second.currentSumAxis[playerIndex];
	}

	return 0.0f;
}

void powe::InputSettings::AddActionMapping(const std::string& name,
	const std::vector<ActionMap::ActionKeyPack>& keyPacks)
{
	const auto findItr{ m_ActionKeyMappings.find(name) };
	if (findItr == m_ActionKeyMappings.end())
	{
		ActionMap actionMap{ keyPacks };
		m_ActionKeyMappings.try_emplace(name, actionMap);
	}
	else
	{
		for (const auto& keyPack : keyPacks)
		{
			findItr->second.AddKey(keyPack);
		}
	}

	for (const auto& keyPack : keyPacks)
	{
		AddKeyToMainKeyPool(keyPack.key);
	}
}

void powe::InputSettings::AddAxisMapping(const std::string& name, const std::vector<AxisMap::AxisKeyPack>& keyPacks)
{
	const auto findItr{ m_AxisKeyMappings.find(name) };
	if (findItr == m_AxisKeyMappings.end())
	{
		AxisMap axisMap{ keyPacks };
		m_AxisKeyMappings.try_emplace(name, axisMap);
	}
	else
	{
		for (const auto& keyPack : keyPacks)
		{
			findItr->second.AddKey(keyPack);
		}
	}

	for (const auto& keyPack : keyPacks)
	{
		AddKeyToMainKeyPool(keyPack.key);
	}
}


bool powe::InputSettings::IsKeyBoardPressed(KeyType key)
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key(key));
}

InputEvent powe::InputSettings::InterpretInputState(bool isKeyPressed, const InputEvent& savedInputState)
{
	if (isKeyPressed)
	{
		switch (savedInputState)
		{

		case InputEvent::IE_Released:
		case InputEvent::IE_None:
			return InputEvent::IE_Pressed;

		case InputEvent::IE_Pressed:	return InputEvent::IE_Down;
		case InputEvent::IE_Down:		return savedInputState;
		}
	}
	else
	{
		switch (savedInputState)
		{
		case InputEvent::IE_Pressed:
		case InputEvent::IE_Down:
			return InputEvent::IE_Released;

		case InputEvent::IE_Released:   return InputEvent::IE_None;
		case InputEvent::IE_None:		return savedInputState;
		}
	}


	return InputEvent::IE_None;
}

void powe::InputSettings::ProcessHWData(const HardwareBus& hardwareBus)
{
	KeyData keyData{};
	switch (EventType(hardwareBus.eventId))
	{
	case EventType::MouseButton:
	{
		const MouseKeyData mouseKey{ std::get<MouseKeyData>(hardwareBus.hData) };
		keyData.axisValue = float(mouseKey.isPressed);
		keyData.isDown = mouseKey.isPressed;
		keyData.key = { hardwareBus.inDevice,mouseKey.keyCode };
		keyData.playerIndex = 0;
	}
	break;
	case EventType::MouseWheelMoved:
	{
		const MouseWheelDelta mouseWheelDelta{ std::get<MouseWheelDelta>(hardwareBus.hData) };
		keyData.axisValue = mouseWheelDelta;
		keyData.isDown = bool(mouseWheelDelta);

		keyData.key = { hardwareBus.inDevice, keyData.axisValue > 0.0f ?
			MouseKey::MK_MiddleUp : MouseKey::MK_MiddleDown };

		keyData.playerIndex = 0;
	}
	break;
	case EventType::MouseMoved:
		//TODO: Do mouse moved event
		break;
	case EventType::KeyboardButton:
	{
		KeyboardData kData{ std::get<KeyboardData>(hardwareBus.hData) };
		keyData.isDown = kData.isPressed;
		keyData.key = { hardwareBus.inDevice,kData.keyCode };
		keyData.axisValue = float(kData.isPressed);
		keyData.playerIndex = 0;
		m_CurrentFrameSystemKey |= kData.sysKey; // syskeys here
	}

	break;
	case EventType::Gamepad: // specialize gamepad handling
	{
		const GamepadData gData{ std::get<GamepadData>(hardwareBus.hData) };

		// Gamepad Buttons Data
		for (int i = 0; i < GamepadKey::ButtonCount; ++i)
		{
			const Key gamepadKey{ InputDevice::D_Gamepad,KeyType(i) };

			if (!m_MainKeyPool[gData.playerIndex].contains(gamepadKey))
				continue;

			KeyData kData{ gamepadKey,gData.playerIndex,0.0f,false };
			if (i == GamepadKey::GPK_Left_Shoulder)
				kData.axisValue = gData.LShoulder;

			else if (i == GamepadKey::GPK_Right_Shoulder)
				kData.axisValue = gData.RShoulder;
			else
				kData.axisValue = float(GamepadKey::GetKeyCodeFromBitPos(i) & gData.buttons);

			kData.isDown = bool(kData.axisValue);

			const InputEvent thisFrameEvent = EvaluateMainKeyPool(kData);
			UpdateAxisMapping(kData, thisFrameEvent);
		}

		// Thumb Axis Data
		for (int i = int(GamepadKey::GPK_Right_AxisX); i < int(GamepadKey::GPK_Count); ++i)
		{
			const Key gamepadKey{ InputDevice::D_Gamepad, KeyType(i) };

			if (!m_MainKeyPool[gData.playerIndex].contains(gamepadKey))
				continue;

			KeyData kData{ gamepadKey,gData.playerIndex,gData.thumbAxisData[GamepadKey::GetThumbIndexDataFromKeyCode(i)] };
			kData.isDown = bool(kData.axisValue);

			const InputEvent thisFrameEvent = EvaluateMainKeyPool(kData);
			UpdateAxisMapping(kData, thisFrameEvent);
		}
	}
	return;
	default:;
	}

	const InputEvent thisFrameEvent = EvaluateMainKeyPool(keyData);
	UpdateAxisMapping(keyData, thisFrameEvent);
}

void powe::InputSettings::AddKeyToMainKeyPool(const Key& key)
{
	if (key.inputDevice == InputDevice::D_Gamepad)
	{
		for (auto& mainKeyPool : m_MainKeyPool)
		{
			mainKeyPool.try_emplace(key, KeyState{});
		}
	}
	else
	{
		// Keyboard and mouse is always assigned to the first player
		m_MainKeyPool[0].try_emplace(key, KeyState{});
	}

}

void powe::InputSettings::UpdateMainKeyPool()
{
	for (auto& playerKeyPool : m_MainKeyPool)
	{
		for (auto& currentState : playerKeyPool | std::views::values)
		{
			currentState.inputLastFrame = currentState.inputThisFrame;
		}
	}
}


InputEvent powe::InputSettings::EvaluateMainKeyPool(const KeyData& inKey)
{
	auto& mainKeyPool{ m_MainKeyPool[inKey.playerIndex] };

	InputEvent thisFrameEvent{ InputEvent::IE_None };

	if (mainKeyPool.contains(inKey.key))
	{
		auto& currentState = mainKeyPool.at(inKey.key);

		currentState.inputThisFrame = inKey.isDown;
		currentState.axisThisFrame = inKey.axisValue;

		thisFrameEvent = InputEvent((currentState.inputLastFrame << 0) | (currentState.inputThisFrame << 1));
	}

	return thisFrameEvent;
}

void powe::InputSettings::UpdateAxisMapping(const KeyData& inKey, InputEvent thisFrameEvent)
{
	if (thisFrameEvent == InputEvent::IE_Pressed ||
		thisFrameEvent == InputEvent::IE_Released)
	{
		for (auto& axisMap : m_AxisKeyMappings)
		{
			// re calculate total axis map
			axisMap.second.currentSumAxis[inKey.playerIndex] = 0;
			for (const auto& [key, scale] : axisMap.second.keyPool)
			{
				if (m_MainKeyPool[inKey.playerIndex].contains(key))
				{
					const auto& currentState{ m_MainKeyPool[inKey.playerIndex].at(key) };
					axisMap.second.currentSumAxis[inKey.playerIndex] += currentState.axisThisFrame * scale;
				}
			}
		}
	}
}

#endif

