#pragma once

#include "POWEngine/Core/CustomTypes.h"
#include "WindowContext.h"
#include "POWEngine/Math/Math.h"

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
		[[nodiscard]] const glm::uvec2& GetRelativeMousePos() const;
		void SetClearColor(const glm::uvec4& color);
		[[nodiscard]] const glm::uvec4& GetClearColor() const { return m_ClearColor; }

		template<typename T>
		T* GetWindowInstance() const;

		~Window();

	protected:

		OwnedPtr<WindowImpl> m_WindowImpl;

		glm::uvec4 m_ClearColor{};
		std::string m_Title;
		uint32_t m_Width;
		uint32_t m_Height;
	};

	template <typename T>
	T* Window::GetWindowInstance() const
	{
		return static_cast<T*>(m_WindowImpl.get());
	}
}



