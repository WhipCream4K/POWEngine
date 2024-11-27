#include "pch.h"
#include "ModulesManager.h"

powe::SharedPtr<powe::IModule> powe::ModulesManager::GetModule(uint32_t id)
{
    const auto it{ m_Modules.find(id) };
    if (it != m_Modules.end())
    {
        return it->second;
    }
    return nullptr;
}