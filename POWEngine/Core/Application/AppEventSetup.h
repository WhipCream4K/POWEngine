#pragma once

namespace powe
{
    class AppEvent;
    class AppEventSetup
    {
        friend class Application;

    private:

        void Sort();

    public:

        using SortValue = std::pair<SharedPtr<AppEvent>,uint32_t>;

        AppEventSetup(Vector<SharedPtr<AppEvent>>& events);

        template<typename T>
        requires std::is_base_of_v<AppEvent, T>
        void Add()
        {
            auto& appEvents{*m_AppEvents};

            // Find the pointer to the type correspond to the AppEvent type
            auto findItr = std::ranges::find_if(appEvents, [](const SharedPtr<AppEvent>& event)
            {
                return std::dynamic_pointer_cast<T>(event) != nullptr;
            });

            if (findItr != appEvents.end())
            {
                m_SortValues.try_emplace(*findItr, m_Priority++);
            }
        }

    private:

        RefWrap<Vector<SharedPtr<AppEvent>>> m_AppEvents;
        UnOrderedMap<SharedPtr<AppEvent>, uint32_t> m_SortValues;
        uint32_t m_Priority{};
    };
}