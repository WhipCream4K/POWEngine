#pragma once

#include "Window/Window.h"
#include "Core/CustomTypes.h"

namespace powe
{
	class WindowManager final
	{
	public:

		WindowManager(PMRResource* memResource);
		~WindowManager();
		Window* CreateWindow(std::string_view title, int width, int height);
		Window* GetMainWindow() const {return m_MainWindow;}
		void DestroyWindow(std::string_view windowName);

		bool Update();

	private:

		UnOrderedMap<std::string,UniquePtr<Window>> m_Windows;
		Window* m_MainWindow;
		PMRResource* m_MemResource;
	};
}