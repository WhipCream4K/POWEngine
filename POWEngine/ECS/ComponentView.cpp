#include "pch.h"
#include "ComponentView.h"


powe::ComponentView::ComponentView(PMRResource* memResource, ECSManager& manager, const Vector<ComponentID>& compIDs)
    : m_ECSManager{manager}
      , m_ComponentsAddresses{memResource}
{
    Vector<IArchetype*> outArchetype{};

    m_ECSManager->GetArchetypes(compIDs, outArchetype);
    for (auto& archetype : outArchetype)
    {
        Vector<void*> componentAddresses{memResource};
        archetype->GetComponents(compIDs, componentAddresses);
        m_ComponentsAddresses.try_emplace(archetype, componentAddresses);
        archetype->InsertInvalidCallback([this](IArchetype* archetype)
        {
            ResetComponentAddresses(archetype);
        });
    }
}

void powe::ComponentView::ResetComponentAddresses(IArchetype* archetype)
{
    if(auto it = m_ComponentsAddresses.find(archetype); it != m_ComponentsAddresses.end())
    {
        archetype->GetComponents(m_ComponentIDs, it->second);
    }
}
