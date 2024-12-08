#include "pch.h"
#include "ComponentView.h"


// powe::ComponentView::ComponentView(ECSManager& manager, const Vector<ComponentID>& compIDs)
//       : m_ComponentsAddresses{manager.GetResource().get()}
// {
//     auto resc{manager.GetResource()};
//     Vector<Archetype*> outArchetype(resc.get());

//     manager.GetArchetypes(compIDs, outArchetype);
//     for (auto& archetype : outArchetype)
//     {
//         Vector<void*> componentAddresses(resc.get());
//         archetype->GetComponents(compIDs, componentAddresses);
//         m_ComponentsAddresses.try_emplace(archetype, componentAddresses);
//         archetype->AddArchetypeInvalidCallback([this](Archetype* archetype)
//         {
//             this->ResetComponentAddresses(archetype);
//         });
//     }
// }

// void powe::ComponentView::ResetComponentAddresses(Archetype* archetype)
// {
//     if(auto it = m_ComponentsAddresses.find(archetype); it != m_ComponentsAddresses.end())
//     {
//         archetype->GetComponents(m_ComponentIDs, it->second);
//     }
// }
