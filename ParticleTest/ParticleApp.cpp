#include "ParticleApp.h"


#include "BoundAreaSystem.h"
#include "EngineStatsComponent.h"
#include "EngineStatsTrackSystem.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Renderer/SFML/SFML2DRenderer.h"
#include "POWEngine/Renderer/System/SFML/SFML2DShapeRenderSystem.h"
#include "TestScene.h"
#include "WanderingSteeringSystem.h"
#include "POWEngine/Renderer/System/SFML/SFMLDefaultRenderSystem.h"

using namespace powe;

void ParticleApp::OnEngineSetUp(powe::EngineProps& engineProps)
{
	engineProps.winProps.width = 1280;
	engineProps.winProps.height = 720;
	engineProps.winProps.windowName = "ParticleTest";
	engineProps.winProps.startWithVSync = false;

	engineProps.renderer->RegisterRenderAPI(std::make_unique<SFML2DRenderer>());
	engineProps.renderer->RegisterSystem(std::make_shared<SFMLDefaultRenderSystem>());
}

void ParticleApp::OnWorldInitialize(powe::WorldEntity& world)
{
	// const GameObjectID engineStats{ world.CreateNewEntity() };
	// world.AddComponentToGameObject(engineStats,EngineStatsComponent{});
	// world.RegisterSystem(PipelineLayer::PostUpdate,std::make_shared<EngineStatsTrackSystem>());
	
	m_TestScene = std::make_shared<TestScene>(world);

	world.RegisterSystem(PipelineLayer::PostUpdate,std::make_shared<BoundAreaSystem>(
		glm::fvec4{0.0f,0.0f,640.0f,480.0f}));

	world.RegisterSystem(PipelineLayer::Update,std::make_shared<WanderingSteeringSystem>());
}
