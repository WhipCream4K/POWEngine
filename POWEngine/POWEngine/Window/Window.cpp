#include "pch.h"
#include "Window.h"

//powe::Window::Window(int width, int height, const std::string& title)
//{
//}


void powe::Window::Resize(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	InternResize(width, height);
}


