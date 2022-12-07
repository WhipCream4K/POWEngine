#pragma once

//#include "POWEngine/Core/CustomTypes.h"
#include <functional>

#include "WindowContext.h"
#include "POWEngine/Math/Math.h"

namespace powe
{
	struct HardwareMessages;
	class WindowImpl;
	class Window
	{
	public:

		using ResizeCallback = std::function<void(uint32_t,uint32_t)>;
		using CreateCallback = std::function<void()>;
		
		Window(uint32_t width, uint32_t height, const std::string& title,uint8_t id,const OtherWindowParams& others = {});

		void PollHardwareMessages(HardwareMessages& hwMessages,bool& shouldEarlyExit, bool& shouldIgnoreInputs) const;
		void Resize(uint32_t width, uint32_t height);
		void SetTitle(const std::string& newTitle);
		void UpdateWindowContext(float deltaTime) const;

		[[nodiscard]] uint32_t GetHeight() const { return m_Height; }
		[[nodiscard]] uint32_t GetWidth() const { return m_Width; }

		void RegisterOnResizeCallback(const ResizeCallback& callback);
		void RegisterOnCreateCallback(const CreateCallback& callback);

		void BroadcastOnCreate() const;
		
		void SetVSync(bool vsync) const;
		void SetFramerateLimit(int fps) const;
		void Display() const;
		void ClearWindow() const;
		void SetClearColor(const glm::uvec4& color) const;
		uint8_t GetID() const {return m_ID;}
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

		std::vector<ResizeCallback> m_OnResizeCallback;
		std::vector<CreateCallback> m_OnCreateCallback;
		
		std::string m_Title;
		uint32_t m_Width;
		uint32_t m_Height;
		uint8_t m_ID{};
	};

	template <typename T>
	T* Window::GetWindowInstance() const
	{
		return static_cast<T*>(m_WindowImpl.get());
	}
}



