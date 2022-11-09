#pragma once
#include "POWEngine/Core/CustomTypes.h"
// #include "POWEngine/ECS/SystemBase.h"

namespace powe
{
    class GameObject;
    class SystemBase;
}

class SceneFactory
{
public:
    
    void AddGameObject(const SharedPtr<powe::GameObject>& gb);
    void AddSystem(const SharedPtr<powe::SystemBase>& system);
    
private:

    std::vector<SharedPtr<powe::GameObject>> m_GameObjects;
    std::vector<SharedPtr<powe::SystemBase>> m_Systems;
};

