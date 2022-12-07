#include "pch.h"
#include "WorldEntity.h"

#include "POWEngine/ECS/Archetype.h"
#include "POWEngine/ECS/SystemBase.h"

powe::WorldEntity::WorldEntity(const SharedPtr<Core>& core)
    : CoreResource(core)
    , AppResource(nullptr)
    , m_SparseComponentManager(*this)
{
}

void powe::WorldEntity::RegisterGameObject(GameObjectID id)
{
    m_GameObjectRecords.try_emplace(id, GameObjectRecord{});
}

powe::WorldEntity::~WorldEntity()
{
    ClearArchetype();
}

void powe::WorldEntity::RemoveSystem(const SharedPtr<SystemBase>& system)
{
    m_PendingDeleteSystem.Push(system);
}

powe::GameObjectID powe::WorldEntity::GetNewEntityID()
{
    return m_GameObjectCounter++;
}

powe::GameObjectID powe::WorldEntity::CreateNewEntity()
{
    const GameObjectID newID{GetNewEntityID()};
    RegisterGameObject(newID);
    return newID;
}

bool powe::WorldEntity::IsGameObjectExist(GameObjectID id) const
{
    return m_GameObjectRecords.contains(id);
}

void powe::WorldEntity::RemoveComponentByID(GameObjectID id, ComponentTypeID componentID)
{
    GameObjectRecord gbRecords{};

    if (GetGameObjectRecords(id, gbRecords))
    {
        // if this gameobject has the archetype that means it got registered in the pipeline
        if (const auto oldArchetype{gbRecords.Archetype.lock()})
        {
            const auto findItr = oldArchetype->ComponentOffsets.find(componentID);
            if (findItr != oldArchetype->ComponentOffsets.end())
            {
                AddComponentToGameObjectRemoveList(id, findItr->first);
            }
        }
        else
        {
            //SharedPtr<SparseComponent> some{new SparseComponent()}
            // find the component id in the pre-archetype list and remove it instantly
            RemoveComponentFromPreArchetype(id, componentID);
        }
    }
}


void powe::WorldEntity::RemoveGameObject(GameObjectID id, bool removeRecord)
{
    GameObjectRecord gbRecords{};
    if (GetGameObjectRecords(id, gbRecords))
    {
        // GameObject can only be deleted if it's in the pipeline
        const auto archetype{gbRecords.Archetype.lock()};
        const int index{gbRecords.IndexInArchetype}; // safe-guard
        if (archetype && index >= 0)
        {
            AddGameObjectToArchetypeRemoveList(CreateStringFromNumVector(archetype->Types), id);
        }
        else
        {
            // Remove all associated components in pre archetype
            RemoveGameObjectFromPreArchetype(id);
        }

        if (removeRecord)
            AddGameObjectToRecordRemoveList(id);
    }
}


void powe::WorldEntity::UpdatePipeline(PipelineLayer layer, float deltaTime)
{
    const auto& systemsInThisPipeline{m_SystemPipeline[int(layer)]};

    for (const auto& system : systemsInThisPipeline)
    {
        for (const auto& archetype : m_ArchetypesPool | std::views::values)
        {
            if (!archetype->GameObjectIds.empty())
            {
                if (IsDigitExistInNumber(archetype->ComponentOffsets, system->GetKeys()))
                    system->Update(*archetype, deltaTime);
            }
        }
    }
}

void powe::WorldEntity::ResolveEntities()
{
    InternalRemoveSystemFromPipeline();
    InternalRemoveGameObjectFromPipeline();
    InternalRemoveComponentFromGameObject();

    InternalAddGameObjectToPipeline();
    InternalAddSystemToPipeline();

    ClearEmptyArchetype();
}

SharedPtr<powe::Archetype> powe::WorldEntity::GetArchetypeByGameObject(GameObjectID id) const
{
    if (m_GameObjectRecords.contains(id))
    {
        return m_GameObjectRecords.at(id).Archetype.lock();
    }

    return nullptr;
}

std::string powe::WorldEntity::CreateStringFromNumVector(const std::vector<ComponentTypeID>& numList)
{
    std::string out{};
    for (const auto& val : numList)
    {
        out.append(std::to_string(val));
    }
    return out;
}

void powe::WorldEntity::InternalRemoveGameObjectFromPipeline()
{
    // Deletes GameObject from archetype
    for (const auto& tobeRemoveGameObjects : m_PendingDeleteGameObjectsFromArchetype | std::views::values)
    {
        SharedPtr<Archetype> revalidateArchetype{};
        std::vector<GameObjectID> gameObjectsIDCopy{};

        // Grab the archetype and save the current game object that are going to be destroyed
        GameObjectRecord tempRec{};
        GetGameObjectRecords(tobeRemoveGameObjects[0], tempRec);
        revalidateArchetype = tempRec.Archetype.lock();
        gameObjectsIDCopy = revalidateArchetype->GameObjectIds;


        for (const GameObjectID removeGameObjectID : tobeRemoveGameObjects)
        {
            GameObjectRecord gbRecords{};
            if (!GetGameObjectRecords(removeGameObjectID, gbRecords))
                continue;

            const auto targetArchetype{gbRecords.Archetype.lock()};

            DestroyAllComponentDataInGameObject(*targetArchetype, gbRecords.IndexInArchetype, removeGameObjectID,
                                                targetArchetype->Types);


            // Remove the GameObject ID out of this archetype
            targetArchetype->GameObjectIds.erase(
                std::ranges::remove(targetArchetype->GameObjectIds, removeGameObjectID).begin(),
                targetArchetype->GameObjectIds.end());

            m_GameObjectRecords.at(removeGameObjectID).IndexInArchetype = -1;
        }

        if (revalidateArchetype)
        {
            if (revalidateArchetype->GameObjectIds.empty())
                continue;

            // 1. find first destroyed GameObject
            int fistRemovedObjectIdx{};

            for (int idx = 0; idx < int(gameObjectsIDCopy.size()); ++idx)
            {
                const auto findItr = std::ranges::find(tobeRemoveGameObjects, gameObjectsIDCopy[idx]);
                if (findItr != tobeRemoveGameObjects.end())
                {
                    fistRemovedObjectIdx = idx;
                    break;
                }
            }

            // Replace the dead block with alive block
            for (int idx = fistRemovedObjectIdx; idx < int(revalidateArchetype->GameObjectIds.size()); ++idx)
            {
                const GameObjectID nextAliveGameObjectID{revalidateArchetype->GameObjectIds[idx]};
                const auto aliveBlockIdx{m_GameObjectRecords.at(nextAliveGameObjectID).IndexInArchetype};

                RawByte* fromAddress{
                    &revalidateArchetype->ComponentData[int(aliveBlockIdx * revalidateArchetype->SizeOfComponentsBlock)]
                };
                RawByte* toAddress{
                    &revalidateArchetype->ComponentData[int(idx * revalidateArchetype->SizeOfComponentsBlock)]
                };


                for (const ComponentTypeID componentID : revalidateArchetype->Types)
                {
                    const SharedPtr<BaseComponent> componentTrait{GetComponentTrait(componentID)};

                    const SizeType offset{revalidateArchetype->ComponentOffsets.at(componentID)};

                    // Move Data over to the new block
                    componentTrait->MoveData(fromAddress + offset,
                                             toAddress + offset);
                }

                m_GameObjectRecords.at(nextAliveGameObjectID).IndexInArchetype = idx;
            }
        }
    }

    // Deletes GameObjects From the records
    for (const auto deleteGameObjectID : m_PendingDeleteGameObjectsFromRecord)
    {
        m_GameObjectRecords.erase(deleteGameObjectID);
    }

    // Clear everything at the end of process
    m_PendingDeleteGameObjectsFromRecord.clear();
    m_PendingDeleteGameObjectsFromArchetype.clear();
}

void powe::WorldEntity::InternalRemoveComponentFromGameObject()
{
    for (const auto& [gameObjectID, componentIDs] : m_PendingDeleteComponentsFromGameObject)
    {
        GameObjectRecord gbRecords{};

        if (!GetGameObjectRecords(gameObjectID, gbRecords))
            continue;

        const auto oldArchetype{gbRecords.Archetype.lock()};
        if (!oldArchetype || gbRecords.IndexInArchetype < 0)
            continue;

        std::vector<ComponentTypeID> newTypes{oldArchetype->Types};

        // remove every types that matches the deleting componentIDs
        newTypes.erase(std::ranges::remove_if(newTypes, [&componentIDs](ComponentTypeID id)
        {
            return std::ranges::find_if(componentIDs, [&id](ComponentTypeID removeID)
            {
                // discard the flag of the component
                return DiscardFlag(removeID) == DiscardFlag(id);
            }) != componentIDs.end();
        }).begin(), newTypes.end());

        const std::string newArchetypeKey{CreateStringFromNumVector(newTypes)};
        const std::string oldArchetypeKey{CreateStringFromNumVector(oldArchetype->Types)};

        auto targetArchetype{GetArchetypeFromActiveList(newArchetypeKey)};

        if (!targetArchetype)
        {
            targetArchetype = Archetype::Create(*this, newTypes);
            m_ArchetypesPool.try_emplace(newArchetypeKey, targetArchetype);
        }

        // 1. if the archetype is valid and is not the same archetype then move the remaining components to another archetype
        if (targetArchetype && targetArchetype != oldArchetype)
        {
            if (const SizeType targetOccupiedSize{
                    SizeType((targetArchetype->GameObjectIds.size() + 1) * targetArchetype->SizeOfComponentsBlock)
                };
                targetOccupiedSize >= targetArchetype->TotalAllocatedData)
            {
                // allocate more space if the current size is equal or more that the total allocated space
                targetArchetype->AllocateComponentData(SizeType(targetOccupiedSize * 3), *this);
            }

            RawByte* fromAddress{
                &oldArchetype->ComponentData[int(gbRecords.IndexInArchetype * oldArchetype->SizeOfComponentsBlock)]
            };
            RawByte* toAddress{
                &targetArchetype->ComponentData[int(
                    targetArchetype->GameObjectIds.size() * targetArchetype->SizeOfComponentsBlock)]
            };

            for (const ComponentTypeID componentTypeId : newTypes)
            {
                const SharedPtr<BaseComponent> componentTrait{GetComponentTrait(componentTypeId)};
                componentTrait->MoveData(
                    fromAddress + oldArchetype->ComponentOffsets.at(componentTypeId),
                    toAddress + targetArchetype->ComponentOffsets.at(componentTypeId));
            }
        }

        // 3. Destroy the component and move over all the data after this gameobject
        {
            DestroyAllComponentDataInGameObject(*oldArchetype,
                gbRecords.IndexInArchetype, gameObjectID, componentIDs);

            oldArchetype->BuryBlock(*this, gbRecords.IndexInArchetype);
        }

        oldArchetype->GameObjectIds.erase(
            std::ranges::remove(oldArchetype->GameObjectIds, gameObjectID).begin(),
            oldArchetype->GameObjectIds.end());

        if (targetArchetype)
        {
            auto& refGORec{GetRefGameObjectRecord(gameObjectID)};

            refGORec.Archetype = targetArchetype;
            refGORec.IndexInArchetype = int(targetArchetype->GameObjectIds.size());

            targetArchetype->GameObjectIds.emplace_back(gameObjectID);
        }
    }

    m_PendingDeleteComponentsFromGameObject.clear();
}

void powe::WorldEntity::ClearArchetype()
{
    for (const auto& archetype : m_ArchetypesPool | std::views::values)
    {
        archetype->CleanUp(*this);
    }
}

void powe::WorldEntity::ClearEmptyArchetype()
{
    for (auto it = m_ArchetypesPool.begin() ; it != m_ArchetypesPool.end();)
    {
        if (it->second->GameObjectIds.empty())
        {
            it->second->CleanUp(*this);
            it = m_ArchetypesPool.erase(it);
        }
        else
            ++it;
    }
}


void powe::WorldEntity::AddGameObjectToRecordRemoveList(GameObjectID id)
{
    if (m_GameObjectRecords.contains(id))
    {
        if (std::ranges::find(m_PendingDeleteGameObjectsFromRecord, id) == m_PendingDeleteGameObjectsFromRecord.end())
            m_PendingDeleteGameObjectsFromRecord.emplace_back(id);
    }
}

void powe::WorldEntity::AddGameObjectToArchetypeRemoveList(const std::string& archetypeKey, GameObjectID id)
{
    const auto findItr{m_PendingDeleteGameObjectsFromArchetype.find(archetypeKey)};
    if (findItr != m_PendingDeleteGameObjectsFromArchetype.end())
    {
        auto& deleteGameObjectList{findItr->second};
        if (std::ranges::find(deleteGameObjectList, id) == findItr->second.end())
            deleteGameObjectList.emplace_back(id);
    }
    else
    {
        std::vector<GameObjectID> temp{};
        temp.emplace_back(id);
        //m_PendingDeleteGameObjectsFromArchetype[archetypeKey] = temp;
        m_PendingDeleteGameObjectsFromArchetype.try_emplace(archetypeKey, temp);
    }
}

void powe::WorldEntity::AddComponentToGameObjectRemoveList(GameObjectID id, ComponentTypeID componentTypeID)
{
    const auto findItr{m_PendingDeleteComponentsFromGameObject.find(id)};
    if (findItr != m_PendingDeleteComponentsFromGameObject.end())
    {
        auto& deleteComponentList{findItr->second};
        if (std::ranges::find(deleteComponentList, componentTypeID) == findItr->second.end())
            deleteComponentList.emplace_back(componentTypeID);
    }
    else
    {
        std::vector<ComponentTypeID> temp{};
        temp.emplace_back(componentTypeID);
        //m_PendingDeleteComponentsFromGameObject[id] = temp;
        m_PendingDeleteComponentsFromGameObject.try_emplace(id, temp);
    }
}

void powe::WorldEntity::AddArchetype(const std::string& key, const SharedPtr<Archetype>& archetype)
{
    m_ArchetypesPool.try_emplace(key, archetype);
}

void powe::WorldEntity::AddPreArchetype(GameObjectID gameObjectID,
                                        ComponentTypeID componentID, const SharedPtr<RawByte[]>& reservedComponentData)
{
    const auto findItr{m_PreArchetypes.find(gameObjectID)};
    if (findItr != m_PreArchetypes.end())
    {
        const auto& [_, result] = findItr->second.componentData.try_emplace(componentID, reservedComponentData);
        if (result)
            findItr->second.archetypeKey.emplace_back(componentID);
    }
    else
    {
        PreArchetypeTrait preArchetypeTrait{};
        preArchetypeTrait.archetypeKey.emplace_back(componentID);
        preArchetypeTrait.componentData.try_emplace(componentID, reservedComponentData);
        m_PreArchetypes.try_emplace(gameObjectID, preArchetypeTrait);
    }
}

void powe::WorldEntity::RemoveComponentFromPreArchetype(GameObjectID id, ComponentTypeID componentTypeId)
{
    const auto gameObjectItr{m_PreArchetypes.find(id)};
    if (gameObjectItr != m_PreArchetypes.end())
    {
        PreArchetypeTrait& archetypeTrait{gameObjectItr->second};
        if (archetypeTrait.componentData.contains(componentTypeId))
        {
            archetypeTrait.componentData.erase(componentTypeId);

            archetypeTrait.archetypeKey.erase(
                std::ranges::remove(archetypeTrait.archetypeKey, componentTypeId).begin(),
                archetypeTrait.archetypeKey.end());
        }
    }
}

void powe::WorldEntity::RemoveGameObjectFromPreArchetype(GameObjectID id)
{
    if (m_PreArchetypes.contains(id))
    {
        for (const auto& [componentID, data] : m_PreArchetypes[id].componentData)
        {
            const ComponentTypeID discardTypeID{DiscardFlag(componentID)};
            const SharedPtr<BaseComponent> thisComponent{GetComponentTrait(discardTypeID)};

            thisComponent->DestroyData(data.get());
        }

        m_PreArchetypes.erase(id);
    }
}

void powe::WorldEntity::DestroyAllComponentDataInGameObject(
    const Archetype& archetype, int index, GameObjectID id,
    const std::vector<ComponentTypeID>& components, bool callInternDestroy)
{
    RawByte* startAddress{&archetype.ComponentData[int(index * archetype.SizeOfComponentsBlock)]};

    for (const ComponentTypeID componentTypeId : components)
    {
        const SizeType offset{archetype.ComponentOffsets.at(componentTypeId)};
        const SharedPtr<BaseComponent> componentTrait{GetComponentTrait(componentTypeId)};

        if (archetype.ComponentOffsets.contains(componentTypeId)) // check if this component id is a sparse component
        {
            m_SparseComponentManager.RemoveComponentFromGameObject(id, DiscardFlag(componentTypeId), callInternDestroy);
        }

        if (callInternDestroy)
            componentTrait->InternalDestroy(startAddress + offset, *this, id);

        componentTrait->DestroyData(startAddress + offset);
    }
}


SharedPtr<powe::BaseComponent> powe::WorldEntity::GetComponentTrait(ComponentTypeID id) const
{
    if (id & int(ComponentFlag::Sparse))
        return SparseComponent::GetStatic();

    if (m_ComponentTraitsMap.contains(id))
        return m_ComponentTraitsMap.at(id);

    return nullptr;
}

SharedPtr<powe::Archetype> powe::WorldEntity::GetArchetypeFromActiveList(const std::string& key) const
{
    if (m_ArchetypesPool.contains(key))
        return m_ArchetypesPool.at(key);

    return nullptr;
}

bool powe::WorldEntity::GetGameObjectRecords(GameObjectID id, GameObjectRecord& outRecord) const
{
    if (m_GameObjectRecords.contains(id))
    {
        outRecord = m_GameObjectRecords.at(id);
        return true;
    }

    return false;
}

powe::GameObjectRecord& powe::WorldEntity::GetRefGameObjectRecord(GameObjectID id)
{
    return m_GameObjectRecords.at(id);
}

bool powe::WorldEntity::GetPreArchetypeTrait(GameObjectID id, PreArchetypeTrait& outTrait) const
{
    if (m_PreArchetypes.contains(id))
    {
        outTrait = m_PreArchetypes.at(id);
        return true;
    }

    return false;
}

void powe::WorldEntity::InternalAddGameObjectToPipeline()
{
    for (const auto& [gameObjectID, preArchetypeTrait] : m_PreArchetypes)
    {
        std::vector<ComponentTypeID> archetypeKey{};
        const auto& componentTempDataMap{preArchetypeTrait.componentData};

        // Check if this gameobject already has its archetype
        bool shouldMoveOldToNew{};
        GameObjectRecord oldRecord{};
        if (GetGameObjectRecords(gameObjectID, oldRecord))
        {
            if (const auto oldArchetype = oldRecord.Archetype.lock())
            {
                archetypeKey = oldArchetype->Types;
                shouldMoveOldToNew = true;
            }
        }

        // We don't need to check for duplicate types because we already check when we add component
        archetypeKey.insert(archetypeKey.end(), preArchetypeTrait.archetypeKey.begin(),
                            preArchetypeTrait.archetypeKey.end());

        const std::string archetypeKeyString{CreateStringFromNumVector(archetypeKey)};

        SharedPtr<Archetype> targetArchetype{GetArchetypeFromActiveList(archetypeKeyString)};
        if (!targetArchetype)
        {
            targetArchetype = Archetype::Create(*this, archetypeKey);
            m_ArchetypesPool.try_emplace(archetypeKeyString, targetArchetype);
        }

        const SizeType objectCnt{SizeType(targetArchetype->GameObjectIds.size())};
        
        const SizeType futureSize{
            (objectCnt + 1)
            * targetArchetype->SizeOfComponentsBlock
        };

        if (futureSize >= targetArchetype->TotalAllocatedData)
        {
            targetArchetype->AllocateComponentData((objectCnt + 2) * 8 * targetArchetype->SizeOfComponentsBlock, *this);
        }


        // if this gameobject already has an archetype then move everything to the new archetype
        int startComponentIdx{};
        if (shouldMoveOldToNew)
        {
            if (const auto oldArchetype = oldRecord.Archetype.lock())
            {
                RawByte* fromAddress{
                    &oldArchetype->ComponentData[int(oldRecord.IndexInArchetype * oldArchetype->SizeOfComponentsBlock)]
                };
                RawByte* toAddress{
                    &targetArchetype->ComponentData[int(
                        targetArchetype->GameObjectIds.size() * targetArchetype->SizeOfComponentsBlock)]
                };

                for (const ComponentTypeID componentTypeId : oldArchetype->Types)
                {
                    const SharedPtr<BaseComponent> componentTrait{GetComponentTrait(componentTypeId)};

                    const SizeType offset{oldArchetype->ComponentOffsets.at(componentTypeId)};
                    componentTrait->MoveData(fromAddress + offset, toAddress + offset);
                }

                startComponentIdx = int(oldArchetype->Types.size());

                oldArchetype->BuryBlock(*this, oldRecord.IndexInArchetype);

                const auto findItr{std::ranges::find(oldArchetype->GameObjectIds, gameObjectID)};
                if (findItr != oldArchetype->GameObjectIds.end())
                    oldArchetype->GameObjectIds.erase(findItr);
            }
        }

        // resume to move data from pre-archetype to the target archetype
        RawByte* destination{
            &targetArchetype->ComponentData[int(
                targetArchetype->GameObjectIds.size() * targetArchetype->SizeOfComponentsBlock)]
        };

        for (int i = startComponentIdx; i < int(archetypeKey.size()); ++i)
        {
            const ComponentTypeID componentTypeId{targetArchetype->Types[i]};
            const ComponentTypeID discardFlagComponentID{DiscardFlag(componentTypeId)};

            const SharedPtr<BaseComponent> componentTrait{GetComponentTrait(componentTypeId)};
            SharedPtr<RawByte[]> compData{componentTempDataMap.at(discardFlagComponentID)};

            // Initialize Component here
            componentTrait->InternalCreate(compData.get(), *this, gameObjectID);

            if (IsThisComponentSparse(componentTypeId))
            {
                m_SparseComponentManager.AddComponentToSparseSet(gameObjectID, discardFlagComponentID, compData);
            }
            else
            {
                RawByte* source{compData.get()};
                componentTrait->MoveData(source, destination + targetArchetype->ComponentOffsets.at(componentTypeId));
            }
        }

        // add the record to this gameobject
        GameObjectRecord gbRecord{};
        gbRecord.Archetype = targetArchetype;
        gbRecord.IndexInArchetype = int(targetArchetype->GameObjectIds.size());

        m_GameObjectRecords[gameObjectID] = gbRecord;

        targetArchetype->GameObjectIds.emplace_back(gameObjectID);
    }

    m_PreArchetypes.clear();
}

void powe::WorldEntity::InternalAddSystemToPipeline()
{
    while (!m_PendingAddSystem.Empty())
    {
        const auto systemTrait{m_PendingAddSystem.Front()};
        m_PendingAddSystem.Pop();
        auto& existingSystems{m_SystemPipeline[int(systemTrait.layer)]};
        if (std::ranges::find(existingSystems, systemTrait.system) == existingSystems.end())
        {
            const auto& system{systemTrait.system};
            for (const auto& archetype : m_ArchetypesPool | std::views::values)
            {
                if (!archetype->GameObjectIds.empty())
                {
                    if (IsDigitExistInNumber(archetype->ComponentOffsets, system->GetKeys()))
                        system->Initialize(*this);
                }
            }

            existingSystems.emplace_back(systemTrait.system);
        }
    }
}

void powe::WorldEntity::InternalRemoveSystemFromPipeline()
{
    std::vector<SharedPtr<SystemBase>> removingSystems{};

    while (!m_PendingDeleteSystem.Empty())
    {
        const auto system{m_PendingDeleteSystem.Front()};
        m_PendingDeleteSystem.Pop();
        removingSystems.emplace_back(system);
    }

    if (!removingSystems.empty())
    {
        for (auto& systemPipeline : m_SystemPipeline)
        {
            const auto findItr{
                std::ranges::remove_if(systemPipeline, [&removingSystems](const SharedPtr<ECSSystemBackend>& activeSystem)
                {
                    return std::ranges::find(removingSystems, activeSystem) != removingSystems.end();
                })
            };

            for (auto it = findItr.begin(); it != findItr.end(); ++it)
            {
                for (const auto& archetype : m_ArchetypesPool | std::views::values)
                {
                    if (!archetype->GameObjectIds.empty())
                    {
                        if (IsDigitExistInNumber(archetype->ComponentOffsets, it->get()->GetKeys()))
                            it->get()->Destroy(*archetype);
                    }
                }
            }


            systemPipeline.erase(findItr.begin(), systemPipeline.end());
        }
    }
}
