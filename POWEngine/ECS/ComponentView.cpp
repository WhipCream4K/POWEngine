#include "pch.h"
#include "ComponentView.h"

using namespace powe;

ComponentView::ComponentView(ECSManager& manager,
    const Vector<ComponentID>& compIDs)
    : m_ComponentIDs(compIDs)
{
    Vector<SharedPtr<Archetype>> outArchetypes{};
    manager.GetArchetypes(compIDs, outArchetypes);

    size_t accumulatedIndex{0};
    for (const auto& arch : outArchetypes)
    {
        Vector<CompAddress> outCompAddress{};
        arch->GetComponents(compIDs, outCompAddress);

        // add to relavant iteration component
        m_ArchetypeViews.emplace_back(ArchetypeViewHandle{arch.get(), accumulatedIndex, outCompAddress.size()});
        accumulatedIndex += outCompAddress.size();

        m_ComponentsAddresses.insert(m_ComponentsAddresses.end(), outCompAddress.begin(), outCompAddress.end());

        // add archetype invalidation callback
        m_InvalidCalleeID = arch->RegisterInvalidCallee([this](Archetype& arch)
        {
            // Still exist run-time
            if(arch.IsValid())
            {
                Vector<CompAddress> outCompAddress{};
                const Vector<ComponentID> compIDs{ this->GetComponentIDs() };
                arch.GetComponents(compIDs, outCompAddress);
                
                // remove the current archetype 
                m_ArchetypeViews.erase(std::remove_if(m_ArchetypeViews.begin(), m_ArchetypeViews.end(), [&arch](const ArchetypeViewHandle& handle)
                {
                    return handle.archetype == &arch;
                }), m_ArchetypeViews.end());

                m_ArchetypeViews.emplace_back(ArchetypeViewHandle{&arch, m_ArchetypeViews.size(), outCompAddress.size()});

                m_ComponentsAddresses.insert(m_ComponentsAddresses.end(), outCompAddress.begin(), outCompAddress.end());
            }
            else
            {
                // only remove the invalid archetype view
                const auto [remFirst,remLast]{ std::ranges::remove_if(m_ArchetypeViews, [&arch](const ArchetypeViewHandle& handle)
                {
                    return handle.archetype == &arch;
                }) };

                const size_t index{remFirst->index};
                const size_t size{remFirst->size};

                m_ComponentsAddresses.erase(m_ComponentsAddresses.begin() + index, m_ComponentsAddresses.begin() + index + size);

                m_ArchetypeViews.erase(remFirst, remLast);
            }
        });
    }
}

ComponentView::~ComponentView()
{
    for (const auto& arch : m_ArchetypeViews)
    {
        if(arch.archetype)
        {
            arch.archetype->RemoveInvalidCallee(m_InvalidCalleeID);
        }
    }
}