#include "pch.h"
#include "ComponentView.h"


powe::ComponentView::ComponentView(PMRResource* memResource, ECSManager& manager, const Vector<ComponentID>& compIDs)
      : m_ComponentsAddresses{memResource}
{
    Vector<IArchetype*> outArchetype(memResource);

    manager.GetArchetypes(compIDs, outArchetype);
    for (auto& archetype : outArchetype)
    {
        Vector<void*> componentAddresses(memResource);
        archetype->GetComponents(compIDs, componentAddresses);
        m_ComponentsAddresses.try_emplace(archetype, componentAddresses);
        archetype->AddArchetypeInvalidCallback([this](IArchetype* archetype)
        {
            this->ResetComponentAddresses(archetype);
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
