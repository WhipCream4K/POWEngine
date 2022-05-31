// BurgerTime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vld.h>

#define USE_SFML_RENDERER 1
#include <powengine.h>
#include "BurgerTimeGame.h"
#include "POWEngine/Logger/Console/ConsoleLogger.h"
#include "POWEngine/Rendering/SFML/SFML2DRenderer.h"
#include "POWEngine/Rendering/System/SFML/SFML2DBoundingBoxRender.h"
#include "POWEngine/Rendering/System/SFML/SFML2DRenderSystem.h"
#include "POWEngine/Rendering/System/SFML/SFMLTextRender.h"
#include "POWEngine/Sound/FMOD/FMOD2DSound.h"


int main()
{
	powe::ServiceLocator::RegisterLogger(std::make_shared<powe::ConsoleLogger>());
	powe::ServiceLocator::RegisterSoundSystem(std::make_shared<powe::FMOD2DSound>(20));

	const SharedPtr<powe::Core> engineCore{ std::make_shared<powe::Core>() };
	const SharedPtr<powe::Window> window{ std::make_shared<powe::Window>(1280,720,"BurgerTime")};
	const SharedPtr<powe::WorldEntity> worldEntity{std::make_shared<powe::WorldEntity>()};

	const SharedPtr<BurgerTimeGame> burgerTimeScene{ std::make_shared<BurgerTimeGame>() };
	//const SharedPtr<powe::WorldClock> worldClock{ engineCore->GetWorldClock() };

	engineCore->RegisterRendererType(std::make_unique<powe::SFML2DRenderer>());

	engineCore->GetRenderer()->RegisterSystem(std::make_shared<powe::SFML2DRenderSystem>());
	engineCore->GetRenderer()->RegisterSystem(std::make_shared<powe::SFMLTextRender>());
	engineCore->GetRenderer()->RegisterSystem(std::make_shared<powe::SFML2DBoundingBoxRender>());

	burgerTimeScene->Start(engineCore,worldEntity);

	engineCore->StartWorldClock();

	while (!engineCore->TranslateWindowInputs(*window,*worldEntity))
	{
		burgerTimeScene->Run(worldEntity,engineCore->GetWorldClock());
		engineCore->Step(*worldEntity);

		engineCore->Draw(*window, *worldEntity);
	}
}
