// BurgerTime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vld.h>

//#include "../POWEngine/POWEngine/Window/Window.h"
#include "POWEngine/Window/Window.h"
#include "POWEngine/Core/Core.h"
#include "POWEngine/Core/CustomTypes.h"
#include "POWEngine/Core/Thread/SimpleThreadPool.h"
#include "POWEngine/Core/LockFree/LFQueue.h"


int main()
{
	powe::Core engineCore{};
	powe::Window window{ 640,480,"NewTitle" };

	while (!engineCore.TranslateWindowInputs(window))
	{

	}

}