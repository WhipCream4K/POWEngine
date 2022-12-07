#include "pch.h"
#include "SystemBase.h"

#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/ECS/Archetype.h"
#include "POWEngine/Logger/LoggerUtils.h"

powe::SystemBase::SystemBase()
    : m_CurrentArchetype()
    , m_UpdateCountPerArchetype()
{
}


void powe::SystemBase::Update(const Archetype& archetype, float deltaTime)
{
    m_UpdateCountPerArchetype = 0;
    m_CurrentArchetype = &archetype;

    try
    {
        for (const auto gameObjectId : archetype.GameObjectIds)
        {
            OnUpdate(deltaTime, gameObjectId);
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

void powe::SystemBase::Destroy(const Archetype& archetype)
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

void powe::SystemBase::InternalUpdate(Archetype* archetype, float deltaTime)
{
    m_UpdateCountPerArchetype = 0;
    m_CurrentArchetype = archetype;

    try
    {
        for (const auto gameObjectId : archetype->GameObjectIds)
        {
            OnUpdate(deltaTime, gameObjectId);
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

// void powe::SystemBase::InternalCreate(WorldEntity* world, const Archetype& archetype)
// {
//     m_UpdateCountPerArchetype = 0;
//     m_CurrentArchetype = &archetype;
//
//     try
//     {
//         for (const auto gameObjectId : archetype.GameObjectIds)
//         {
//             OnCreate(gameObjectId);
//             ++m_UpdateCountPerArchetype;
//         }
//
//         OnPostCreate();
//     }
//     catch (const std::exception& e)
//     {
//         std::string errMsg{"System trying to access component that doesn't exist in this archetype -> "};
//         errMsg.append(e.what());
//         POWLOGWARNING(errMsg);
//
//         m_UpdateCountPerArchetype = 0;
//         m_CurrentArchetype = nullptr;
//     }
// }

void powe::SystemBase::InternalDestroy(const Archetype& archetype)
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

powe::RawByte* powe::SystemBase::GetComponentAddress(ComponentTypeID id, SizeType componentSize) const
{
    return GetComponentAddressByID(*m_CurrentArchetype, id, componentSize);
}

powe::RawByte* powe::SystemBase::GetComponentAddressByID(const Archetype& archetype, ComponentTypeID id,
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
