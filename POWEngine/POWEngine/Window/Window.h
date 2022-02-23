#pragma once

#include "POWEngine/Core/CustomTypes.h"
#include "WindowContext.h"

namespace powe
{
	class WindowImpl;
	class Window final
	{
	public:

		Window(uint32_t width, uint32_t height, const std::string& title,const OtherWindowParams& others = {});

		const WindowMessages& PollWindowMessages(bool& shouldEarlyExit, bool& shouldIgnoreInputs) const;
		const HardwareMessages& PollHardwareMessages(bool& shouldEarlyExit, bool& shouldIgnoreInputs) const;
		void Resize(uint32_t width, uint32_t height);
		void SetTitle(const std::string& newTitle);

		[[nodiscard]] uint32_t GetHeight() const { return m_Height; }
		[[nodiscard]] uint32_t GetWidth() const { return m_Width; }

		~Window();

	protected:

		OwnedPtr<WindowImpl> m_WindowImpl;

		std::string m_Title;
		uint32_t m_Width;
		uint32_t m_Height;
	};
}



