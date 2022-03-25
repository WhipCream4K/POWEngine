#include "pch.h"
#include "WorldEntity.h"

#include "POWEngine/Core/ECS/Archetype.h"
#include "POWEngine/Core/ECS/SystemBase.h"

powe::WorldEntity::WorldEntity() = default;

void powe::WorldEntity::RemoveSystem(const SharedPtr<SystemBase>& system) const
{
	const auto& targetPipeline{ m_SystemPipeline[size_t(system->GetPipeLineLayer())] };
	const auto& itr = std::ranges::find(targetPipeline, system); // ranges::find since c++20
	if (itr != targetPipeline.end())
	{
		itr->get()->MarkDeleted(true);
	}
}

powe::GameObjectId powe::WorldEntity::GetEntityId()
{
	return m_GameObjectCounter++;
}

void powe::WorldEntity::RemoveComponentByID(GameObjectId id, ComponentTypeID componentID)
{

}

void powe::WorldEntity::RemoveGameObject(GameObjectId id)
{
	const auto findItr{ m_GameObjectRecords.find(id) };

	if (findItr != m_GameObjectRecords.end())
	{
		if(const SharedPtr<Archetype> oldArchetype{ findItr->second.Archetype.lock() })
		{
			const std::string archetypeKey{ CreateStringFromNumVector(oldArchetype->Types) };
			const auto archetypeItr{ m_PendingArchetypesMap.find(archetypeKey) };

			SharedPtr<Archetype> targetArchetype{};
			if (archetypeItr != m_PendingArchetypesMap.end())
			{
				// if we have the archetype in the pending then good
				// we can change it immediately
				targetArchetype = archetypeItr->second;
			}
			else
			{
				// we need to allocate new archetype and move over the data
				targetArchetype = std::make_shared<Archetype>();
				targetArchetype->Copy(*oldArchetype);
			}

			
		}
	}
}

void powe::WorldEntity::UpdatePipeline(PipelineLayer layer, float deltaTime)
{
	const auto& systemsInThisPipeline{ m_SystemPipeline[size_t(layer)] };

	for (const auto& system : systemsInThisPipeline)
	{
		for (const auto& [_,archetype] : m_ArchetypesPool)
		{
			if(!archetype->GameObjectIds.empty())
			{
				if (IsDigitExistInNumber(archetype->Types, system->GetKeys()))
					system->InternalUpdate(*this, *archetype, deltaTime);
			}
			//if(!archetype->GameObjectIds.empty())
			//{
			//	if (IsDigitExistInNumber(archetype->Types, system->GetKeys()))
			//		system->InternalUpdate(*this, *archetype,deltaTime);
			//}
		}
	}
}

bool powe::WorldEntity::IsDigitExistInNumber(const std::vector<ComponentTypeID>& compIds,
	const std::unordered_set<ComponentTypeID>& digit)
{
	for (const auto& id : compIds)
	{
		// this checks also take into account the hierarchy of the components
		const ComponentTypeID number{ id & ~(1u << ChildOfBitPos) }; // clearing the child of bit flag
		if (!digit.contains(number))
			return false;
	}

	return true;
}

SharedPtr<powe::Archetype> powe::WorldEntity::CreateArchetypeWithTypes(const std::vector<ComponentTypeID>& typeID)
{
	// assume that the given keys is sorted
	const std::string key{ CreateStringFromNumVector(typeID) };

	if(!m_PendingArchetypesMap.contains(key))
	{
		const SharedPtr<Archetype> archetype{ std::make_shared<Archetype>() };
		m_PendingArchetypesMap[key] = archetype;
	}

	return m_PendingArchetypesMap[key];
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

SharedPtr<powe::Archetype> powe::WorldEntity::UpdatePendingArchetypeKey(const std::string& targetKey,
                                                                        const std::string& newKey)
{
	auto nodeHandle{ m_PendingArchetypesMap.extract(targetKey) };
	SharedPtr<Archetype> out{};

	if(!nodeHandle.empty())
	{
		nodeHandle.key() = newKey;
		out = nodeHandle.mapped();
		m_PendingArchetypesMap.insert(std::move(nodeHandle));
		return out;
	}

	return out;
}

