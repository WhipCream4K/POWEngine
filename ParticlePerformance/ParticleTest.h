#pragma once

#include "powengine.h"

class ParticleTest : public powe::Application
{
public:
    void OnEngineSetUp(powe::EngineProps& engineProps) override;
    void OnWorldInitialize() override;
};
