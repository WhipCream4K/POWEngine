#pragma once
#include "SceneFactory.h"
#include "POWEngine/Core/CustomTypes.h"

namespace powe
{
    class GameObject;
    class WorldEntity;
}

class TestScene
{
public:

    TestScene(powe::WorldEntity& world);

    SharedPtr<powe::GameObject> GetSceneObject() const {return m_SceneObject;}
    
private:

    SharedPtr<powe::GameObject> m_SceneObject;
    SharedPtr<powe::GameObject> m_AgentVerticesBatch{};
};
