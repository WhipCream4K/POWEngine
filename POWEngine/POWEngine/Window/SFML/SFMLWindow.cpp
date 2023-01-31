#include "pch.h"
#include "SFMLWindow.h"

#include "POWEngine/Core/Input/Key.h"
#include "POWEngine/Core/Input/ListsOfKeys.h"

#include "POWEngine/Debug/imgui/ImGUI.h"

#if USE_SFML_WINDOW

powe::SFMLWindow::SFMLWindow(uint32_t width, uint32_t height, const std::string& title, OtherWindowParams others)
    : WindowImpl(width, height, title, others)
      , m_HWMessages()
      , m_WndHandle(
          sf::VideoMode(width, height),
          sf::String{title.c_str()},
          static_cast<sf::Uint32>(others[0]),
          reinterpret_cast<const sf::ContextSettings&>(others[sizeof(sf::Uint32)]))
      // , m_MousePosLastPoll()
      , m_ClearColor(0, 0, 0, 255)
    , m_WindowSize(width,height)
// , m_DeltaMousePos()
{
    const auto mousePos = sf::Mouse::getPosition(m_WndHandle);
    // m_MousePosLastPoll.x = mousePos.x;
    // m_MousePosLastPoll.y = mousePos.y;

    // InitDebugWindowContext();
}

powe::SFMLWindow::SFMLWindow(uint32_t width, uint32_t height, const std::string& title)
    : WindowImpl(width, height, title)
      , m_HWMessages()
      , m_WndHandle(sf::VideoMode(width, height), sf::String{title.c_str()})
      // , m_MousePosLastPoll()
      , m_ClearColor(0, 0, 0, 255)
, m_WindowSize(width,height)
// , m_DeltaMousePos()
{
    const auto mousePos = sf::Mouse::getPosition(m_WndHandle);
    // m_MousePosLastPoll.x = mousePos.x;
    // m_MousePosLastPoll.y = mousePos.y;
    // InitDebugWindowContext();
}

void powe::SFMLWindow::PollHardwareMessages(
    HardwareMessages& hardwareMessages,
    bool& shouldEarlyExit,
    bool& shouldIgnoreInputs)
{
    int messageCnt{hardwareMessages.totalMessages};

    if (m_WndHandle.isOpen())
    {
        sf::Event sfmlEvent{};
        while (m_WndHandle.pollEvent(sfmlEvent))
        {
            if (messageCnt > MinimumWindowEventCnt)
                break;

            HardwareData messageData{};

            switch (sfmlEvent.type)
            {
            case sf::Event::Closed:
                {
                    m_WndHandle.close();
                    shouldEarlyExit = true;
                    // return;
                }
            case sf::Event::GainedFocus:
                {
                    break;
                }
            case sf::Event::LostFocus:
                {
                    shouldIgnoreInputs = true;
                    // return;
                }
            case sf::Event::KeyPressed:
                {
                    KeyboardData keyboardData{uint8_t(sfmlEvent.key.code)};
                    keyboardData.sysKey = uint8_t(sfmlEvent.key.alt << int(KeyboardSysKey::KS_Alt) |
                        sfmlEvent.key.control << int(KeyboardSysKey::KS_Ctrl) |
                        sfmlEvent.key.shift << int(KeyboardSysKey::KS_Shift) |
                        sfmlEvent.key.system << int(KeyboardSysKey::KS_System));

                    keyboardData.isPressed = true;
                    messageData.hData = keyboardData;
                    messageData.eventId = uint8_t(EventType::KeyboardButton);
                    messageData.inDevice = InputDevice::D_Keyboard;

                    hardwareMessages.hwMessages[messageCnt++] = messageData;

                    break;
                }
            case sf::Event::KeyReleased:
                {
                    KeyboardData keyboardData{uint8_t(sfmlEvent.key.code)};
                    keyboardData.sysKey = uint8_t(sfmlEvent.key.alt << int(KeyboardSysKey::KS_Alt) |
                        sfmlEvent.key.control << int(KeyboardSysKey::KS_Ctrl) |
                        sfmlEvent.key.shift << int(KeyboardSysKey::KS_Shift) |
                        sfmlEvent.key.system << int(KeyboardSysKey::KS_System));

                    keyboardData.isPressed = false;

                    messageData.hData = keyboardData;
                    messageData.eventId = uint8_t(EventType::KeyboardButton);
                    messageData.inDevice = InputDevice::D_Keyboard;

                    hardwareMessages.hwMessages[messageCnt++] = messageData;

                    break;
                }
            case sf::Event::Resized:
                {
                    Resize(sfmlEvent.size.width, sfmlEvent.size.height);
                    break;
                }
            case sf::Event::MouseButtonPressed:
                {
                    messageData.hData = MouseKeyData{uint8_t(sfmlEvent.mouseButton.button), true};
                    messageData.eventId = uint8_t(EventType::MouseButton);
                    messageData.inDevice = InputDevice::D_Mouse;

                    hardwareMessages.hwMessages[messageCnt++] = messageData;

                    break;
                }
            case sf::Event::MouseButtonReleased:
                {
                    messageData.hData = MouseKeyData{uint8_t(sfmlEvent.mouseButton.button), false};
                    messageData.eventId = uint8_t(EventType::MouseButton);
                    messageData.inDevice = InputDevice::D_Mouse;

                    hardwareMessages.hwMessages[messageCnt++] = messageData;

                    break;
                }
            case sf::Event::MouseWheelScrolled:
                {
                    // messageData.hData = MouseWheelDelta(sfmlEvent.mouseWheelScroll.delta);
                    messageData.eventId = uint8_t(EventType::MouseWheelMoved);
                    messageData.inDevice = InputDevice::D_Mouse;
                    hardwareMessages.mouseAxis.deltaWheel = sfmlEvent.mouseWheelScroll.delta;
                    hardwareMessages.hwMessages[messageCnt++] = messageData;

                    break;
                }
            case sf::Event::MouseMoved:
                {
                    messageData.eventId = uint8_t(EventType::MouseMoved);
                    messageData.inDevice = InputDevice::D_Mouse;

                    hardwareMessages.mouseAxis.mousePos.x = sfmlEvent.mouseMove.x;
                    hardwareMessages.mouseAxis.mousePos.y = sfmlEvent.mouseMove.y;

                    hardwareMessages.mouseAxis.windowDimension = m_WindowSize;

                    // m_DeltaMousePos.x = float(sfmlEvent.mouseMove.x - m_MousePosLastPoll.x);
                    // m_DeltaMousePos.y = float(m_MousePosLastPoll.y - sfmlEvent.mouseMove.y);

                    // MousePos mousePos = MousePos{
                    // 	m_DeltaMousePos.x,
                    // 	m_DeltaMousePos.y,
                    // 	sfmlEvent.mouseMove.x,
                    // 	sfmlEvent.mouseMove.y };
                    //
                    // messageData.hData = mousePos;
                    //
                    // m_MousePosLastPoll.x = sfmlEvent.mouseMove.x;
                    // m_MousePosLastPoll.y = sfmlEvent.mouseMove.y;

                    hardwareMessages.hwMessages[messageCnt++] = messageData;

                    break;
                }

            default: break;
            }
        }

        ParseHWMessageToDebugWindow(sfmlEvent);
        hardwareMessages.totalMessages = messageCnt;
    }
}

void powe::SFMLWindow::Resize(uint32_t width, uint32_t height)
{
    m_WndHandle.setSize({width, height});
    m_WindowSize.x = width;
    m_WindowSize.y = height;
}

void powe::SFMLWindow::SetTitle(const std::string& title)
{
    m_WndHandle.setTitle(sf::String{title.c_str()});
}

void powe::SFMLWindow::ClearWindow()
{
    sf::Uint8 r{uint8_t(m_ClearColor.x)};
    sf::Uint8 g{uint8_t(m_ClearColor.y)};
    sf::Uint8 b{uint8_t(m_ClearColor.z)};
    sf::Uint8 a{uint8_t(m_ClearColor.w)};

    m_WndHandle.clear({r, g, b, a});
}

void powe::SFMLWindow::SetClearColor(const glm::uvec4& color)
{
    m_ClearColor = color;
}

void powe::SFMLWindow::Display()
{
    RenderDebugWindowContext();
    // m_WndHandle.display();
}

void powe::SFMLWindow::SetVSync(bool VSync)
{
    m_WndHandle.setVerticalSyncEnabled(VSync);
}

void powe::SFMLWindow::UpdateWindowContext(float deltaTime)
{
    UpdateDebugWindowContext(deltaTime);
}

void powe::SFMLWindow::SetFramerateLimit(int fps)
{
    m_WndHandle.setFramerateLimit(fps);
}

const glm::uvec4& powe::SFMLWindow::GetClearColor() const
{
    return m_ClearColor;
}

powe::SFMLWindow::~SFMLWindow()
{
    m_WndHandle.close();
}

void powe::SFMLWindow::ParseHWMessageToDebugWindow(sf::Event& ev [[maybe_unused]]) const
{
#if USE_IMGUI
    ImGui::SFML::ProcessEvent(m_WndHandle, ev);
    // ImGui::NewFrame();
#endif
}

void powe::SFMLWindow::UpdateDebugWindowContext(float deltaTime [[maybe_unused]])
{
#if USE_IMGUI
    ImGui::SFML::Update(m_WndHandle, sf::seconds(deltaTime));
#endif
}

void powe::SFMLWindow::InitDebugWindowContext()
{
#if USE_IMGUI
    ImGui::SFML::Init(m_WndHandle);
#endif
}

void powe::SFMLWindow::RenderDebugWindowContext()
{
      #if USE_IMGUI
          // ImGui::ShowDemoWindow();
          ImGui::SFML::Render( m_WndHandle);
      #endif
}


#endif
