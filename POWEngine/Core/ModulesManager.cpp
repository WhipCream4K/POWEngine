#include "pch.h"
#include "ModulesManager.h"

using namespace powe;

SharedPtr<IModule> ModulesManager::GetModule(uint32_t id) const noexcept
{
    const auto it{ m_Modules.find(id) };
    if (it != m_Modules.end())
    {
        auto& [ module, _ ] = it->second;
        return module;
    }
    return nullptr;
}

PMRResource* ModulesManager::GetModuleResource(uint32_t id) const noexcept
{
    const auto it{ m_Modules.find(id) };
    if (it != m_Modules.end())
    {
        auto& [ _, resource ] = it->second;
        return resource;
    }
    return GetAppResource();
}

void ModulesManager::Clear()
{
    for (auto& module : m_Modules)
    {
        module.second.first->OnExit(this);
    }

}

