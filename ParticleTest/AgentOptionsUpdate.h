#pragma once

#include "poweSystem.h"
#include "UserComponents.h"

namespace powe
{
    class GameObject;
}

class AgentOptionsUpdate : public powe::SystemBase 
{
public:
    AgentOptionsUpdate(const SharedPtr<powe::GameObject>& debugObject);
protected:
    void OnCreate(powe::GameObjectID) override;
    void OnUpdate(float, powe::GameObjectID) override;
private:

    WeakPtr<powe::GameObject> m_DebugObject{};
    DebugSteeringComponent* m_DebugSteering{};
};
