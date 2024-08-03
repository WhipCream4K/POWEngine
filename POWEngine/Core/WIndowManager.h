#pragma once

#include "Core/Window.h"

namespace powe
{
	class WindowManager final
	{
	public:

		Window* GetMainWindow() const { return m_Windows[0].get(); }

	private:

		Vector<SharedPtr<Window>> m_Windows;
	};
}