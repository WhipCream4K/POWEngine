// BurgerTime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vld.h>

#include "POWEngine/Window/Window.h"
#include "POWEngine/Core/Core.h"


int main()
{
	powe::Core engineCore{};
	const powe::Window window{ 640,480,"NewTitle" };

	engineCore.StartWorldClock();

	while (!engineCore.TranslateWindowInputs(window))
	{

	}

}