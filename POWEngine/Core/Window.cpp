#include "pch.h"
#include "Window.h"

powe::Window::Window(std::pmr::memory_resource* memResource, const std::string& title, int width, int height)
	: m_MemResource(memResource), m_Title(title), m_Width(width), m_Height(height)
{
}

powe::Window::~Window()
{
}

void powe::Window::SetFullscreen(bool fullscreen)
{
	
}
