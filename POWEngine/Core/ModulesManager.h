#pragma once

#include "Core/IModule.h"

namespace powe
{

    template<typename T>
    concept ModuleConcept = std::is_base_of_v<IModule, T>;
}


namespace powe
{
    class ModulesManager
    {
    public:

        void AddModule(UniquePtr<IModule>&& module);
        void RemoveModule(std::string_view moduleName);

        void AddModuleDependencies(std::string_view moduleName, const Vector<std::string>& dependencies);

        template<ModuleConcept T>
        T* GetModule(std::string_view moduleName)
        {
            for (auto& module : m_Modules)
            {
                if (module->GetName() == moduleName)
                {
                    return static_cast<T*>(module.get());
                }
            }

            return nullptr;
        }
        
    private:

        Vector<UniquePtr<IModule>> m_Modules;
    };
}

