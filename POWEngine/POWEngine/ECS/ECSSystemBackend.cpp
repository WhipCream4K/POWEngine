#include "pch.h"
#include "ECSSystemBackend.h"

#include "POWEngine/Logger/LoggerUtils.h"


void powe::ECSSystemBackend::Initialize(WorldEntity& worldEntity,const Archetype& archetype)
{
    m_World = &worldEntity;
    OnCreate();

    m_UpdateCountPerArchetype = 0;
    m_CurrentArchetype = &archetype;

    try
    {
        for (const auto gameObjectId : archetype.GameObjectIds)
        {
            OnPerGameObjectCreate(gameObjectId);
            ++m_UpdateCountPerArchetype;
        }
    }
    catch (const std::exception& e)
    {
        std::string errMsg{"System trying to access component that doesn't exist in this archetype -> "};
        errMsg.append(e.what());
        POWLOGWARNING(errMsg);

        m_UpdateCountPerArchetype = 0;
        m_CurrentArchetype = nullptr;
    }
}

void powe::ECSSystemBackend::Destroy(const Archetype& archetype)
{
    m_UpdateCountPerArchetype = 0;
    m_CurrentArchetype = &archetype;

    try
    {
        for (const auto gameObjectId : archetype.GameObjectIds)
        {
            OnDestroy(gameObjectId);
            ++m_UpdateCountPerArchetype;
        }
    }
    catch (const std::exception& e)
    {
        std::string errMsg{"System trying to access component that doesn't exist in this archetype -> "};
        errMsg.append(e.what());
        POWLOGWARNING(errMsg);

        m_UpdateCountPerArchetype = 0;
        m_CurrentArchetype = nullptr;
    }
}


powe::RawByte* powe::ECSSystemBackend::GetComponentAddress(ComponentTypeID id, SizeType componentSize) const
{
    return GetComponentAddressByID(*m_CurrentArchetype, id, componentSize);
}

powe::RawByte* powe::ECSSystemBackend::GetComponentAddressByID(const Archetype& archetype, ComponentTypeID id,
                                                         SizeType componentSize) const
{
    const auto findItr = archetype.ComponentOffsets.find(id);

    if (findItr != archetype.ComponentOffsets.end())
    {
        // check the key if it's a sparse component or not
        if (!IsThisComponentSparse(findItr->first))
        {
            RawByte* dataAddress{
                &archetype.ComponentData[
                    m_UpdateCountPerArchetype * archetype.SizeOfComponentsBlock
                    + findItr->second // offsets
                ]
            };

            return dataAddress;
        }

        // if it is Sparse component
        // get the data from sparse section
        auto& sparseManager{m_World->GetSparseComponentManager()};

        return sparseManager.GetComponentData(
            archetype.GameObjectIds[m_UpdateCountPerArchetype], id, componentSize);
    }

    std::string errMsg{"There is no component ID: " + std::to_string(id)};
    throw std::out_of_range(errMsg);
}
