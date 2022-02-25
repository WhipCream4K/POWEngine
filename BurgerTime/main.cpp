// BurgerTime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vld.h>

#include "POWEngine/Window/Window.h"
#include "POWEngine/Core/Core.h"
#include "POWEngine/Core/Clock/WorldClock.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"


int main()
{
	powe::Core engineCore{};
	const powe::Window window{ 640,480,"NewTitle" };
	SharedPtr<powe::WorldEntity> worldEntity{std::make_shared<powe::WorldEntity>()};

	engineCore.StartWorldClock();

	//float hello = worldEntity->GetInputSettings().GetAxis("Some");
	//hello;
	while (!engineCore.TranslateWindowInputs(window,worldEntity))
	{

	}

}