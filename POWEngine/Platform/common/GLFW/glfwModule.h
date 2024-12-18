#pragma once

#include "Core/IModule.h"
#include "Core/Application/AppEvent.h"

namespace powe
{

    class glfwEvent : public AppEvent
    {
    public:

        virtual void OnSetup() override {}
        virtual void OnUpdate(float) override;  

    };


    class glfwModule : public IModule
    {
    public:

        glfwModule();
        ~glfwModule() override = default;

        virtual void OnCreate(ModulesManager*) override;
        virtual void OnExit(ModulesManager*) override;

    private:

        SharedPtr<glfwEvent> m_GlfwEvent;
    };
}