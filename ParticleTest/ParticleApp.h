#pragma once

#include "powengine.h"

class ParticleApp final : public powe::Application
{

public:

	ParticleApp() = default;
	~ParticleApp() override = default;

	void OnEngineSetUp(powe::EngineProps&) override;
	void OnWorldInitialize(powe::WorldEntity&) override;
};

