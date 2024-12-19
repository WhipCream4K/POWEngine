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

        template<ModuleConcept T>
        SharedPtr<T> GetModule() // User should save as WeakPtr because modules can be unloaded any time
        {
            const uint32_t moduleID{ IModule::GetID<T>() };
            const auto it{ m_Modules.find(moduleID) };
            if (it != m_Modules.end())
            {
                return std::static_pointer_cast<T>(it->second.first);
            }
            return nullptr;
        }

        SharedPtr<IModule> GetModule(uint32_t id) const noexcept;

        template<ModuleConcept T,typename ...Args>
        SharedPtr<T> CreateModule(PMRResource* memResource = nullptr,Args&&... args) noexcept
        {
            const uint32_t moduleID{ IModule::GetID<T>() };
            const auto it{ m_Modules.find(moduleID) };
            if (it != m_Modules.end())
            {
                return std::static_pointer_cast<T>(it->second.first);
            }

            // PMRResource* resc{memResource.get()};

            if(memResource == nullptr)
            {
                memResource = GetAppResource();
            }

            SharedPtr<T> module{AllocateShared<T>(memResource, std::forward<Args>(args)...)};
            m_Modules.try_emplace(moduleID, std::make_pair(module, memResource));
            module->OnCreate(this);

            return module;
        }

        PMRResource* GetModuleResource(uint32_t id) const noexcept;

        template<ModuleConcept T>
        PMRResource* GetModuleResource() const noexcept
        { 
            return GetModuleResource(IModule::GetID<T>()); 
        }

        void Clear();
        
    private:

        // The allocator will exist for the lifetime of the application
        // since removing allocators during runtime is not feasible for run-time environment
        // so if we are going to store allocators reference it should be a raw pointer
        // to save memory overhead and ease of use and the only class that manages the allocators
        // should be the MemoryManager
        using ModulePair = std::pair<SharedPtr<IModule>, PMRResource*>;

        UnOrderedMap<uint32_t,ModulePair> m_Modules;
        uint32_t m_CurrentActiveModules;
    };
}

