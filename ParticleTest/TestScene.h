#pragma once
#include "SceneFactory.h"
#include "POWEngine/Core/CustomTypes.h"

namespace powe
{
    class GameObject;
    class WorldEntity;
}

class TestScene : public SceneFactory
{
public:

    TestScene(powe::WorldEntity& world);

private:

    SharedPtr<powe::GameObject> m_GameObjects;
};
