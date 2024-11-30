#pragma once

namespace powe
{
    class Scene;
    class SceneEvent
    {
    public:
        SceneEvent() = default;
        virtual ~SceneEvent() = default;

        virtual void OnCreate(Scene&) {}
        virtual void OnUpdate(Scene&) {}
    };
}