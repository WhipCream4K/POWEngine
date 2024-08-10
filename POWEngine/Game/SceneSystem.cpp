#include "pch.h"
#include "SceneSystem.h"

#include "ECS/ECSUtils.h"

powe::ComponentView& powe::SceneSystem::GetCachedComponentView(const Vector<ComponentID>& compIDs)
{
    return std::ranges::find_if(m_CacheComponentViews,[&compIDs](const auto& pair)
    {
        return IsArchetypeMatch(pair.first, compIDs);
    })->second;
}
