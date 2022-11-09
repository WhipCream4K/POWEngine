#pragma once

#include <POWEngine/Math/Math.h>

#include "poweSystem.h"

struct DebugSteeringComponent;

namespace powe
{
    class GameObject;
}

class BoundAreaSystem : public powe::SystemBase
{
public:

    BoundAreaSystem(const glm::fvec4& box);
    BoundAreaSystem(const SharedPtr<powe::GameObject>& gb);

    void OnCreate(powe::GameObjectID) override;
    void OnUpdate(float, powe::GameObjectID) override;
    
private:

    WeakPtr<powe::GameObject> m_BoundArea{};
    DebugSteeringComponent* m_DebugSteeringOpt{};
    glm::fvec4 m_BoundingBox{};
};
