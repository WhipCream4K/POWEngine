#pragma once

#include "Core/CustomTypes.h"

namespace powe
{
    class Scene;
    class SceneManager
    {
    public:
        
        SceneManager(PMRResource* memResource = DefaultAllocator::Engine);
        
        Scene& CreatScene(std::string_view sceneName) noexcept;
        void RemoveScene(std::string_view sceneName) noexcept;

        Scene* GetScene(std::string_view sceneName) noexcept;
        
    private:

        UnOrderedMap<std::string,UniquePtr<Scene>> m_Scenes;
        PMRResource* m_DefaultAllocator;
    };    
}

