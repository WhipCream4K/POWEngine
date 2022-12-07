#pragma once

#include "poweSystem.h"

class CellRenderer;
class CellUpdateSystem : public powe::SystemBase
{
public:

    CellUpdateSystem();

protected:

    void OnUpdate(float, powe::GameObjectID) override;
    
private:
    
    // std::future<void> m_ThreadRender{};

    std::vector<SharedPtr<CellRenderer>> m_CellRenderers{};
};
