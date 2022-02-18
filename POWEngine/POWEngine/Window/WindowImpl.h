#pragma once

#include "WindowContext.h"

namespace powe
{
	// Platform specific windows
	class WindowImpl
	{
	public:

		WindowImpl(uint32_t width, uint32_t height, const std::string& title);
		WindowImpl(uint32_t width, uint32_t height, const std::string& title,const OtherWindowParams& others);
		virtual const WindowMessages& PollWindowMessages(bool& shouldEarlyExit) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual ~WindowImpl();

	protected:

		WindowMessages m_WndMessages;
	};
}


