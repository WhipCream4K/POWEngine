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

    BroadcastOnCreate();
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
    BroadcastOnResize();
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

uint32_t powe::Window::RegisterOnResizeCallback(const ResizeCallback& callback)
{
    const uint32_t handle{m_OnResizeHandle++};
    m_OnResizeCallback.emplace_back(handle,callback);
    return handle;
}

void powe::Window::RemoveOnResizeCallback(uint32_t handle)
{
    if(m_OnResizeCallback.empty())
        return;
        
    const auto ret{std::ranges::remove_if(m_OnResizeCallback,[&handle](const ResizePair& pair)
    {
        return handle == pair.first;
    })};

    m_OnResizeCallback.erase(ret.begin(),ret.end());
}

uint32_t powe::Window::RegisterOnCreateCallback(const CreateCallback& callback)
{
    const uint32_t handle{m_OnCreateHandle++};
    m_OnCreateCallback.emplace_back(handle,callback);
    return handle;
}

void powe::Window::BroadcastOnCreate() const
{
    for (const auto& callback : m_OnCreateCallback | std::views::values)
    {
        callback(*this);
    }
}

void powe::Window::BroadcastOnResize() const
{
    for (const auto& callback : m_OnResizeCallback | std::views::values)
    {
        callback(*this);
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

std::mutex& powe::Window::GetWindowMutex() const
{
    return m_WindowImpl->GetWindowMutex();
}

powe::Window::~Window() = default;
