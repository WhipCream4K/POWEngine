#include "pch.h"
#include "Frame.h"

#include <GLFW/glfw3.h>

#include "Core/Application/Application.h"

void powe::Frame::Bind()
{
    Application::Get().GetWindowManager().GetMainWindow();
}

void powe::Frame::Clear()
{
    
}
