#include "ParticleApp.h"

#include "POWEngine/Renderer/SFML/SFML2DRenderer.h"

using namespace powe;

void ParticleApp::OnEngineSetUp(powe::EngineProps& engineProps)
{
	engineProps.winProps.width = 1280;
	engineProps.winProps.height = 720;
	engineProps.winProps.windowName = "ParticleTest";
	engineProps.winProps.startWithVSync = false;

	engineProps.renderer->RegisterRenderAPI(std::make_unique<SFML2DRenderer>());
}

void ParticleApp::OnWorldInitialize(powe::WorldEntity& )
{

}
