#pragma once
#include "POWEngine/Core/CustomTypes.h"

namespace powe
{
    class GameObject;
}

class SceneFactory
{
public:

    void AddGameObject(const SharedPtr<powe::GameObject>& gb);
    
private:

    std::vector<SharedPtr<powe::GameObject>> m_GameObjects;
};

