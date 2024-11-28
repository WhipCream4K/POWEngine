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

        virtual void OnCreate(ModulesManager*) = 0;
        virtual void OnExit(ModulesManager*) = 0;
        
        std::string_view GetName() const { return m_Name; }

        template<typename T> requires std::is_base_of_v<IModule, T>
        static uint32_t GetID() 
        { 
            static const uint32_t id{m_ModulesID++};
            return id;
        }
        
    private:

        static std::atomic_uint32_t m_ModulesID;

        std::string m_Name;
    };

    template<typename T>
    concept ModuleConcept = std::is_base_of_v<IModule, T>;
}

