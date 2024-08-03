#pragma once

namespace powe
{
    template<typename T>
    class SubSystem
    {
    public:
        static std::string SystemType() { return typeid(T).name(); }
    };
}
