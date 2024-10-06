#pragma once

namespace powe
{
    class ModulesManager;
    class IModule
    {
    public:

        IModule(std::string_view name)
        : m_Name(name)
        {
        }
        
        virtual ~IModule() = default;

        virtual void OnStartUp(ModulesManager*) = 0;
        virtual void OnExit(ModulesManager*) = 0;
        virtual void OnUpdate(float deltaTime) = 0;
        virtual bool ShouldUpdate() const { return true; }
        
        const std::string& GetName() const { return m_Name; }
        
    private:

        std::string m_Name;
        std::vector<std::string> m_Dependencies;
    };
}

