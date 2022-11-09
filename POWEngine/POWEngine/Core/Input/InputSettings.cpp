#include "pch.h"
#include "InputSettings.h"

#include "InputStruct.h"
#include "POWEngine/Logger/LoggerUtils.h"

#if USE_SFML_WINDOW

#include <SFML/Window.hpp>

void powe::InputSettings::ParseHWMessages(const HardwareMessages& hwMessages)
{
    // we need syskeys alive through this frame
    // to check against all the key that needs them
    m_LastFrameSystemKey = m_CurrentFrameSystemKey;

    UpdateMainKeyPool();

    for (int i = 0; i < hwMessages.totalMessages; ++i)
    {
        // const HardwareData& hwBus{ hwMessages.hwMessages[i] };
        ProcessHWData(hwMessages, i);
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
    const auto findItr{m_AxisKeyMappings.find(axisName)};
    if (findItr != m_AxisKeyMappings.end())
    {
        return findItr->second.currentSumAxis[playerIndex];
    }

    return 0.0f;
}

bool powe::InputSettings::GetInputAction(const std::string& actionName, InputEvent targetEvent,
                                         uint8_t playerIndex) const
{
    assert(playerIndex < MAXPLAYER && "Player index isn't not in range of max player");
    const auto findItr{m_ActionKeyMappings.find(actionName)};
    if (findItr != m_ActionKeyMappings.end())
    {
        for (const auto& actionKey : findItr->second.keyPool)
        {
            if (m_MainKeyPool[playerIndex].contains(actionKey))
            {
                const auto& keyState = m_MainKeyPool[playerIndex].at(actionKey);
                const InputEvent thisFrameEvent = InputEvent(
                    (keyState.inputLastFrame << 0) | (keyState.inputThisFrame << 1));

                if (thisFrameEvent == targetEvent)
                    return true;
            }
        }
    }

    return false;
}

void powe::InputSettings::AddActionMapping(const std::string& name,
                                           const std::vector<ActionMap::ActionKeyPack>& keyPacks)
{
    const auto findItr{m_ActionKeyMappings.find(name)};
    if (findItr == m_ActionKeyMappings.end())
    {
        ActionMap actionMap{keyPacks};
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
    const auto findItr{m_AxisKeyMappings.find(name)};
    if (findItr == m_AxisKeyMappings.end())
    {
        AxisMap axisMap{keyPacks};
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

void powe::InputSettings::SetAssignFirstControllerToNextPlayer(bool state)
{
    m_AssignFirstControllerToNextIndex = state;
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

        case InputEvent::IE_Pressed: return InputEvent::IE_Down;
        case InputEvent::IE_Down: return savedInputState;
        }
    }
    else
    {
        switch (savedInputState)
        {
        case InputEvent::IE_Pressed:
        case InputEvent::IE_Down:
            return InputEvent::IE_Released;

        case InputEvent::IE_Released: return InputEvent::IE_None;
        case InputEvent::IE_None: return savedInputState;
        }
    }


    return InputEvent::IE_None;
}

void powe::InputSettings::ProcessHWData(const HardwareMessages& hardwareMessages, int id)
{
    KeyData keyData{};
    const HardwareData& hwData{hardwareMessages.hwMessages[id]};
    switch (EventType(hwData.eventId))
    {
    case EventType::MouseButton:
        {
            const MouseKeyData mouseKey{std::get<MouseKeyData>(hwData.hData)};
            keyData.axisValue = float(mouseKey.isPressed);
            keyData.isDown = mouseKey.isPressed;
            keyData.key = {hwData.inDevice, mouseKey.keyCode};
            keyData.playerIndex = 0;
        }
        break;
    case EventType::MouseWheelMoved:
        {
            // mouse wheel up
            if (hardwareMessages.mouseAxis.deltaWheel > 0.0f)
            {
                KeyData customKey{};
                customKey.key = {InputDevice::D_Mouse, MouseKey::MK_MiddleUp};
                customKey.axisValue = hardwareMessages.mouseAxis.deltaWheel;
                customKey.isDown = true;
                customKey.playerIndex = 0;
                const InputEvent thisFrameEvent{EvaluateMainKeyPool(customKey)};
                UpdateAxisMapping(customKey, thisFrameEvent);
            }
            else
            {
                KeyData customKey{};
                customKey.key = {InputDevice::D_Mouse, MouseKey::MK_MiddleDown};
                customKey.axisValue = hardwareMessages.mouseAxis.deltaWheel;
                customKey.isDown = true;
                customKey.playerIndex = 0;
                const InputEvent thisFrameEvent{EvaluateMainKeyPool(customKey)};
                UpdateAxisMapping(customKey, thisFrameEvent);
            }
        }
        return;
    case EventType::MouseMoved:
        {
            // mouse moved  has 2 keys
            const glm::fvec2 halfWinDim{
                float(hardwareMessages.mouseAxis.windowDimension.x) * 0.5f,
                float(hardwareMessages.mouseAxis.windowDimension.y) * 0.5f
            };

            const float horizontalPos = float(hardwareMessages.mouseAxis.mousePos.x) - halfWinDim.x;
            const float verticalPos = halfWinDim.y - float(hardwareMessages.mouseAxis.mousePos.y);

            if(abs(m_MouseAxisData.x - horizontalPos) > 0.001f)
            {
                // mouse X
                KeyData customKey{};
                customKey.key = {InputDevice::D_Mouse, MouseKey::MK_AxisX};
                customKey.axisValue = (m_MouseAxisData.x - horizontalPos) / halfWinDim.x;
                customKey.isDown = true;
                customKey.playerIndex = 0;
                InputEvent thisFrameEvent{EvaluateMainKeyPool(customKey)};
                UpdateAxisMapping(customKey, thisFrameEvent);
            }

            if(abs(m_MouseAxisData.y - verticalPos) > 0.001f)
            {
                // mouse Y
                KeyData customKey{};
                customKey.key = {InputDevice::D_Mouse, MouseKey::MK_AxisY};
                customKey.axisValue = (m_MouseAxisData.x - horizontalPos) / halfWinDim.y;
                customKey.isDown = true;
                customKey.playerIndex = 0;
                InputEvent thisFrameEvent{EvaluateMainKeyPool(customKey)};
                UpdateAxisMapping(customKey, thisFrameEvent);
            }
            
            m_MouseAxisData = {horizontalPos, verticalPos};
        }
        return;
    case EventType::KeyboardButton:
        {
            KeyboardData kData{std::get<KeyboardData>(hwData.hData)};
            keyData.isDown = kData.isPressed;
            keyData.key = {hwData.inDevice, kData.keyCode};
            keyData.axisValue = float(kData.isPressed);
            keyData.playerIndex = 0;
            m_CurrentFrameSystemKey |= kData.sysKey; // syskeys here
        }
        break;
    case EventType::Gamepad: // specialize gamepad handling
        {
            const GamepadData gData{std::get<GamepadData>(hwData.hData)};

            const uint8_t shouldAssignFirstControllerToNextIdx{uint8_t(m_AssignFirstControllerToNextIndex)};

            // Gamepad Buttons Data
            for (int i = 0; i < GamepadKey::ButtonCount; ++i)
            {
                const Key gamepadKey{InputDevice::D_Gamepad, KeyType(i)};

                const uint8_t playerIndex{uint8_t(gData.playerIndex + shouldAssignFirstControllerToNextIdx)};

                //if (!m_MainKeyPool[gData.playerIndex].contains(gamepadKey))
                if (!m_MainKeyPool[playerIndex].contains(gamepadKey))
                    continue;

                //KeyData kData{ gamepadKey,gData.playerIndex,0.0f,false };
                KeyData kData{gamepadKey, playerIndex, 0.0f, false};
                if (i == GamepadKey::GPK_Left_Shoulder)
                    kData.axisValue = gData.LShoulder;

                else if (i == GamepadKey::GPK_Right_Shoulder)
                    kData.axisValue = gData.RShoulder;
                else
                    kData.axisValue = float(bool(GamepadKey::GetKeyCodeFromBitPos(i) & gData.buttons));

                kData.isDown = bool(kData.axisValue);

                const InputEvent thisFrameEvent = EvaluateMainKeyPool(kData);
                UpdateAxisMapping(kData, thisFrameEvent);
            }

            // Thumb Axis Data
            for (int i = int(GamepadKey::GPK_Right_AxisX); i < int(GamepadKey::GPK_Count); ++i)
            {
                const Key gamepadKey{InputDevice::D_Gamepad, KeyType(i)};
                const uint8_t playerIndex{uint8_t(gData.playerIndex + shouldAssignFirstControllerToNextIdx)};

                //if (!m_MainKeyPool[gData.playerIndex].contains(gamepadKey))
                if (!m_MainKeyPool[playerIndex].contains(gamepadKey))
                    continue;

                //KeyData kData{ gamepadKey,gData.playerIndex,gData.thumbAxisData[GamepadKey::GetThumbIndexDataFromKeyCode(i)] };
                KeyData kData{
                    gamepadKey, playerIndex, gData.thumbAxisData[GamepadKey::GetThumbIndexDataFromKeyCode(i)]
                };
                kData.isDown = bool(kData.axisValue);

                const InputEvent thisFrameEvent = EvaluateMainKeyPool(kData);
                UpdateAxisMapping(kData, thisFrameEvent);
            }
        }
        return;
    default: ;
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
    auto& mainKeyPool{m_MainKeyPool[inKey.playerIndex]};

    InputEvent thisFrameEvent{InputEvent::IE_None};

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
                    const auto& currentState{m_MainKeyPool[inKey.playerIndex].at(key)};
                    axisMap.second.currentSumAxis[inKey.playerIndex] += currentState.axisThisFrame * scale;
                }
            }
        }
    }
}

#endif
