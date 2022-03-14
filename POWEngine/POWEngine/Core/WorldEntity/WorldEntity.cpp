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

void powe::WorldEntity::RemoveComponentFromGameObject(GameObjectId id, ComponentTypeID componentID)
{

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
		if (digit.contains(number))
			return true;
	}

	return false;
}

SharedPtr<powe::Archetype> powe::WorldEntity::CreateArchetypeWithTypes(const std::vector<ComponentTypeID>& typeID)
{
	// assume that the given keys is sorted
	const std::string key{ CreateStringFromNumVector(typeID) };
	
	if(!m_PendingArchetypesMap.contains(key))
	{
		SharedPtr<Archetype> archetype{ std::make_shared<Archetype>() };
		archetype->Types = typeID;
		m_PendingArchetypesMap[key] = archetype;
		return archetype;
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

