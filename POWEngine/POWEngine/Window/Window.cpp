#include "pch.h"
#include "Window.h"

#include "POWEngine/Logger/LoggerUtils.h"

#if USE_SFML_WINDOW
#include "SFML/SFMLWindow.h"
using WindowType = powe::SFMLWindow;
#endif

powe::Window::Window(uint32_t width, uint32_t height, const std::string& title, const OtherWindowParams& others)
	: m_WindowImpl()
	, m_Title(title)
	, m_Width(width)
	, m_Height(height)
{
	if (others.empty())
		m_WindowImpl = std::make_unique<WindowType>(width, height, title);
	else
		m_WindowImpl = std::make_unique<WindowType>(width, height, title, others);

	if(m_WindowImpl)
	{
		std::string log{ "Window Creation: using -> " };
		log.append(typeid(WindowType).name());
		POWLOGINFO(log);
	}
}

const powe::WindowMessages& powe::Window::PollWindowMessages(bool& shouldEarlyExit, bool& shouldIgnoreInputs) const
{
	return m_WindowImpl->PollWindowMessages(shouldEarlyExit, shouldIgnoreInputs);
}

const powe::HardwareMessages& powe::Window::PollHardwareMessages(bool& shouldEarlyExit, bool& shouldIgnoreInputs) const
{
	return m_WindowImpl->PollHardwareMessages(shouldEarlyExit, shouldIgnoreInputs);
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

//const glm::vec2& powe::Window::GetDeltaMousePos() const
//{
//	
//}

const glm::uvec2& powe::Window::GetRelativeMousePos() const
{
	return m_WindowImpl->GetRelativeMousePos();
}

void powe::Window::ClearWindow() const
{
	
}

void powe::Window::SetClearColor(const glm::uvec4& color)
{
	m_ClearColor = color;
}

powe::Window::~Window() = default;


