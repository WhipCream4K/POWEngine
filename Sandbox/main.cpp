
#include "Core/Application/Application.h"
#include <iostream>

int main()
{
    powe::Application application{powe::AppDesc{"POWE Sandbox", 1280, 720}};
    application.Run();
    return 0;
}