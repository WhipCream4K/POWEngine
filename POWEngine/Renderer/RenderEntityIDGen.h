#pragma once

namespace powe
{
    template <typename T>
    concept RenderState = std::is_standard_layout_v<T>;

    template<typename T>
    concept CGetRenderState = std::is_same_v<std::invoke_result_t<T>,void*>;
    
    class RenderEntityIDGen final
    {
    public:
        
        template<RenderState state>
        static uint32_t Get()
        {
            static const uint32_t id{GenerateID()};
            return id;
        }

        static size_t Size()
        {
            return m_Counter;
        }

    private:
        
        static uint32_t GenerateID()
        {
            return m_Counter++;
        }

        static std::atomic<uint32_t> m_Counter;
    };
}
