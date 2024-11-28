#pragma once

#include <functional>

namespace powe
{
    // struct Input
    // {
    //     enum State
    //     {
    //         Pressed,
    //         Released,
    //         Held
    //     };

    //     struct KeyBinding
    //     {
    //         int key;
    //         State state;
    //         std::function<void()> callback;
    //     };
    // };
    
    class InputManager
    {
    public:

        InputManager(const SharedPtr<PMRResource>& memResource);
        
        void RegisterKeyBinding(int key, std::function<void()> callback);


    private:

        
        SharedPtr<PMRResource> m_DefaultResource;
    };
}


