#pragma once

#include <string>
#include <functional>

namespace powe
{
	class Window
	{
	public:

		Window(std::pmr::memory_resource* memResource,const std::string& title, int width, int height);
		~Window();

		void Initialize();
		void PollEvents();
		void SwapBuffers();
		void SetFullscreen(bool fullscreen);
		void SetTitle(const std::string& title);
		void Resize(int width, int height);


		int GetWidth() const;
		int GetHeight() const;
		bool IsFullscreen() const;

	private:

		// Private Methods
		void CreateWindow();
		void DestroyWindow();

		// Private Members
		std::string m_Title;
		std::pmr::memory_resource* m_MemResource;

		int m_Width;
		int m_Height;
		bool m_Fullscreen;


		void* m_WindowHandle;
	};
}
