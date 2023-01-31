#pragma once

#include <functional>
#include <mutex>

#include "WindowContext.h"
#include "POWEngine/Math/Math.h"
#include "POWEngine/Core/CustomTypes.h"

namespace powe
{
	struct HardwareMessages;
	class WindowImpl;
	class Window
	{
	public:
		
		using ResizeCallback = std::function<void(const Window&)>;
		using CreateCallback = std::function<void(const Window&)>;

		using ResizePair = std::pair<uint32_t,ResizeCallback>;
		using CreatePair = std::pair<uint32_t,CreateCallback>;
		
		Window(uint32_t width, uint32_t height, const std::string& title,uint8_t id,const OtherWindowParams& others = {});

		void PollHardwareMessages(HardwareMessages& hwMessages,bool& shouldEarlyExit, bool& shouldIgnoreInputs) const;
		void Resize(uint32_t width, uint32_t height);
		void SetTitle(const std::string& newTitle);
		void UpdateWindowContext(float deltaTime) const;

		[[nodiscard]] uint32_t GetHeight() const { return m_Height; }
		[[nodiscard]] uint32_t GetWidth() const { return m_Width; }

		uint32_t RegisterOnResizeCallback(const ResizeCallback& callback);
		void RemoveOnResizeCallback(uint32_t handle);
		uint32_t RegisterOnCreateCallback(const CreateCallback& callback);

		void BroadcastOnCreate() const;
		void BroadcastOnResize() const;
		
		void SetVSync(bool vsync) const;
		void SetFramerateLimit(int fps) const;
		void Display() const;
		void ClearWindow() const;
		void SetClearColor(const glm::uvec4& color) const;
		uint8_t GetID() const {return m_ID;}
		[[nodiscard]] const glm::uvec4& GetClearColor() const;
		std::mutex& GetWindowMutex() const;

		template<typename T>
		T* GetWindowInstance() const;

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&&) = default;
		Window& operator=(Window&&) = default;
		virtual ~Window();

	protected:

		OwnedPtr<WindowImpl> m_WindowImpl;
		
		std::vector<ResizePair> m_OnResizeCallback;
		uint32_t m_OnResizeHandle{};
		std::vector<CreatePair> m_OnCreateCallback;
		uint32_t m_OnCreateHandle{};
		
		std::string m_Title;
		uint32_t m_Width;
		uint32_t m_Height;
		uint8_t m_ID{};
	};

	template <typename T>
	T* Window::GetWindowInstance() const
	{
		return dynamic_cast<T*>(m_WindowImpl.get());
	}
}



