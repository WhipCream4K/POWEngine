#include "pch.h"
#include "Window.h"

#include "POWEngine/Logger/LoggerUtils.h"

#if USE_SFML_WINDOW
#include "SFML/SFMLWindow.h"
using WindowType = powe::SFMLWindow;
#else
using WindowType = void*;
#endif

powe::Window::Window(uint32_t width, uint32_t height, const std::string& title, uint8_t id,
                     const OtherWindowParams& others)
    : m_WindowImpl()
      , m_Title(title)
      , m_Width(width)
      , m_Height(height)
      , m_ID(id)
{
    if (others.empty())
        m_WindowImpl = std::make_unique<WindowType>(width, height, title);
    else
        m_WindowImpl = std::make_unique<WindowType>(width, height, title, others);

    if (m_WindowImpl)
    {
        std::string log{"Window Creation: using -> "};
        log.append(typeid(WindowType).name());
        POWLOGINFO(log);
    }
}

void powe::Window::PollHardwareMessages(HardwareMessages& hwMessages, bool& shouldEarlyExit,
                                        bool& shouldIgnoreInputs) const
{
    m_WindowImpl->PollHardwareMessages(hwMessages, shouldEarlyExit, shouldIgnoreInputs);
}

void powe::Window::Resize(uint32_t width, uint32_t height)
{
    m_Width = width;
    m_Height = height;

    m_WindowImpl->Resize(width, height);
}

void powe::Window::SetTitle(const std::string& newTitle)
{
    m_Title = newTitle;

    m_WindowImpl->SetTitle(newTitle);
}

void powe::Window::UpdateWindowContext(float deltaTime) const
{
    m_WindowImpl->UpdateWindowContext(deltaTime);
}

void powe::Window::RegisterOnResizeCallback(const ResizeCallback& callback)
{
    m_OnResizeCallback.emplace_back(callback);
}

void powe::Window::RegisterOnCreateCallback(const CreateCallback& callback)
{
    m_OnCreateCallback.emplace_back(callback);
}

void powe::Window::BroadcastOnCreate() const
{
    for (const auto& callback : m_OnCreateCallback)
    {
        callback();
    }
}

void powe::Window::SetVSync(bool vsync) const
{
    m_WindowImpl->SetVSync(vsync);
}

void powe::Window::SetFramerateLimit(int fps) const
{
    m_WindowImpl->SetFramerateLimit(fps);
}

void powe::Window::Display() const
{
    m_WindowImpl->Display();
}

void powe::Window::ClearWindow() const
{
    m_WindowImpl->ClearWindow();
}

void powe::Window::SetClearColor(const glm::uvec4& color) const
{
    m_WindowImpl->SetClearColor(color);
}

const glm::uvec4& powe::Window::GetClearColor() const
{
    return m_WindowImpl->GetClearColor();
}

powe::Window::~Window() = default;
