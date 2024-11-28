#include "pch.h"
#include "AppEventSetup.h"

using namespace powe;

AppEventSetup::AppEventSetup(Vector<SharedPtr<AppEvent>>& events)
    : m_AppEvents(events)
{
}

void AppEventSetup::Sort() const noexcept
{
    auto& appEvents{*m_AppEvents};

    std::ranges::sort(appEvents, [this](const SharedPtr<AppEvent>& left, const SharedPtr<AppEvent>& right)
    {
        return m_SortValues.at(left) < m_SortValues.at(right);
    });
}