#pragma once

#include "powengine.h"

class SFMLAgentRenderSystem;
class TestScene;
class ParticleApp final : public powe::Application
{

public:

	ParticleApp() = default;
	~ParticleApp() override = default;

	void OnEngineSetUp(powe::EngineProps&) override;
	void OnWorldInitialize(powe::WorldEntity&) override;

private:

	SharedPtr<TestScene> m_TestScene;
};

