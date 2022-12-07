#pragma once

#include "WindowContext.h"

namespace powe
{
	// Platform specific windows
	struct HardwareMessages;
	class WindowImpl
	{
	public:

		WindowImpl(uint32_t width, uint32_t height, const std::string& title);
		WindowImpl(uint32_t width, uint32_t height, const std::string& title,const OtherWindowParams& others);
		// virtual const HardwareMessages& PollHardwareMessages(bool& shouldEarlyExit, bool& shouldIgnoreInputs) = 0;
		virtual void PollHardwareMessages(HardwareMessages& hwMessages, bool& shouldEarlyExit, bool& shouldIgnoreInputs) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void SetTitle(const std::string& title) = 0;
		virtual void ClearWindow() = 0;
		virtual void Display() = 0;
		virtual void SetClearColor(const glm::uvec4&) = 0;
		virtual const glm::uvec4& GetClearColor() const = 0;
		virtual void SetVSync(bool) = 0;
		virtual void UpdateWindowContext(float deltaTime) = 0;
		virtual void SetFramerateLimit(int fps) = 0;
		// [[nodiscard]] virtual const glm::uvec2& GetRelativeMousePos() const = 0;

		virtual ~WindowImpl();

	protected:


		//glm::uvec2 m_MousePosLastPoll;
	};
}


