#include "ParticleTest.h"

#include "EngineStatsComponent.h"
#include "EngineStatsSystem.h"
#include "POWEngine/Renderer/SFML/SFML2DRenderer.h"

using namespace powe;

void ParticleTest::OnEngineSetUp(EngineProps& engineProps)
{
    engineProps.winProps.width = 1280;
    engineProps.winProps.height = 720;
    engineProps.winProps.windowName = "ParticleTest";
    engineProps.winProps.startWithVSync = false;
    
    engineProps.renderer->RegisterRenderAPI(std::make_unique<SFML2DRenderer>());
}

void ParticleTest::OnWorldInitialize(powe::WorldEntity& worldEntity)
{
    const SharedPtr<GameObject> statsComponent{std::make_shared<GameObject>(worldEntity)};
    statsComponent->AddComponent(EngineStatsComponent{});

    worldEntity.RegisterSystem(PipelineLayer::PostUpdate,std::make_shared<EngineStatsSystem>());
}
