#pragma once

#include "Core/IModule.h"
#include "Utils/Utils.h"

namespace powe
{
    class ModulesManager
    {
    public:

        // Adding modules independently so others can manage the modules outside before adding to the modules map
        template<ModuleConcept T>
        void AddModule(const SharedPtr<T>& module)
        {
            const uint32_t moduleID{ IModule::GetID<T>() };
            m_Modules.try_emplace(moduleID, module);
        }

        void RemoveModule(uint32_t id);

        template<ModuleConcept T>
        SharedPtr<T> GetModule() // User should save as WeakPtr because modules can be unloaded any time
        {
            const uint32_t moduleID{ IModule::GetID<T>() };
            const auto it{ m_Modules.find(moduleID) };
            if (it != m_Modules.end())
            {
                return std::static_pointer_cast<T>(it->second);
            }
            return nullptr;
        }

        SharedPtr<IModule> GetModule(uint32_t id);

        template<ModuleConcept T,typename ...Args>
        SharedPtr<T> CreateModule(const SharedPtr<PMRResource>& memResource = nullptr,Args&&... args)
        {
            const uint32_t moduleID{ IModule::GetID<T>() };
            const auto it{ m_Modules.find(moduleID) };
            if (it != m_Modules.end())
            {
                return std::static_pointer_cast<T>(it->second);
            }

            SharedPtr<PMRResource> resc{ memResource }; // Copy the resource

            if(resc == nullptr)
            {
                resc = GetAppResource();
            }

            SharedPtr<T> module{ std::allocate_shared<T>(resc, std::forward<Args>(args)...) };
            m_Modules.try_emplace(moduleID, std::make_pair(module, resc));
            return module;
        }

        SharedPtr<PMRResource> GetModuleResource(uint32_t id);

        template<ModuleConcept T>
        SharedPtr<PMRResource> GetModuleResource() 
        { 
            return GetModuleResource(IModule::GetID<T>()); 
        }
        
    private:

        using ModulePair = std::pair<SharedPtr<IModule>, SharedPtr<PMRResource>>;

        UnOrderedMap<uint32_t,ModulePair> m_Modules;
        // UnOrderedMap<uint32_t,SharedPtr<IModule>> m_UnloadedModules>
        uint32_t m_CurrentActiveModules;
    };
}

