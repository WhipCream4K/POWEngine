#include "ParticleApp.h"

#include "EngineStatsComponent.h"
#include "EngineStatsTrackSystem.h"
#include "POWEngine/Renderer/SFML/SFML2DRenderer.h"

using namespace powe;

void ParticleApp::OnEngineSetUp(powe::EngineProps& engineProps)
{
	engineProps.winProps.width = 1280;
	engineProps.winProps.height = 720;
	engineProps.winProps.windowName = "ParticleTest";
	engineProps.winProps.startWithVSync = false;

	engineProps.renderer->RegisterRenderAPI(std::make_unique<SFML2DRenderer>());
	// engineProps.renderer->RegisterSystem(std::make_shared<EngineStatsTrackSystem>());
}

void ParticleApp::OnWorldInitialize(powe::WorldEntity& world)
{
	const GameObjectID engineStats{ world.CreateNewEntity() };
	world.AddComponentToGameObject(engineStats,EngineStatsComponent{});
	world.RegisterSystem(PipelineLayer::PostUpdate,std::make_shared<EngineStatsTrackSystem>());
}
