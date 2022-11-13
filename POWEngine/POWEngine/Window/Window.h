#pragma once

//#include "POWEngine/Core/CustomTypes.h"
#include "WindowContext.h"
#include "POWEngine/Math/Math.h"

namespace powe
{
	struct HardwareMessages;
	class WindowImpl;
	class Window
	{
	public:

		Window(uint32_t width, uint32_t height, const std::string& title,const OtherWindowParams& others = {});

		// const HardwareMessages& PollHardwareMessages(bool& shouldEarlyExit, bool& shouldIgnoreInputs) const;
		void PollHardwareMessages(HardwareMessages& hwMessages,bool& shouldEarlyExit, bool& shouldIgnoreInputs) const;
		void Resize(uint32_t width, uint32_t height);
		void SetTitle(const std::string& newTitle);
		void UpdateWindowContext(float deltaTime) const;

		[[nodiscard]] uint32_t GetHeight() const { return m_Height; }
		[[nodiscard]] uint32_t GetWidth() const { return m_Width; }
		// [[nodiscard]] const glm::uvec2& GetRelativeMousePos() const;

		void SetVSync(bool vsync) const;
		void Display() const;
		void ClearWindow() const;
		void SetClearColor(const glm::uvec4& color) const;
		[[nodiscard]] const glm::uvec4& GetClearColor() const;

		template<typename T>
		T* GetWindowInstance() const;

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&&) = default;
		Window& operator=(Window&&) = default;
		virtual ~Window();

	protected:

		OwnedPtr<WindowImpl> m_WindowImpl;

		//glm::uvec4 m_ClearColor{};
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



