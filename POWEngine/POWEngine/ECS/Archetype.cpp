#include "pch.h"
#include "Archetype.h"

#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

powe::Archetype::Archetype()
	: Types()
	, GameObjectIds()
	, ComponentData()
	, SizeOfComponentsBlock()
	, TotalAllocatedData(64)
{
	ComponentData = SharedPtr<RawByte[]>{ new RawByte[TotalAllocatedData]{} };
}

SharedPtr<powe::Archetype> powe::Archetype::Create(const WorldEntity& world, const std::vector<ComponentTypeID>& types)
{
	SharedPtr<Archetype> archetype{ std::make_shared<Archetype>() };

	SizeType offset{};
	for (const ComponentTypeID componentTypeId : types)
	{
		const SharedPtr<BaseComponent> thisComponent{ world.GetComponentTrait(componentTypeId) };
		const SizeType componentSize{ thisComponent->GetSize() };
		archetype->Types.emplace_back(componentTypeId);
		archetype->ComponentOffsets.try_emplace(componentTypeId, offset);
		archetype->SizeOfComponentsBlock += componentSize;
		offset += componentSize;
	}

	return archetype;
}

powe::RawByte* powe::Archetype::GetPointer(int pointerDiff) const
{
	return &ComponentData[pointerDiff];
}

SharedPtr<powe::RawByte[]> powe::Archetype::CopyComponentData(const Archetype& other, const WorldEntity& world) const
{

	SharedPtr<RawByte[]> newComponentData{ SharedPtr<RawByte[]>{new RawByte[other.TotalAllocatedData]{}} };

	for (int i = 0; i < int(GameObjectIds.size()); ++i)
	{
		RawByte* startAddress{ &other.ComponentData[int(i * SizeOfComponentsBlock)] };
		RawByte* endAddress{ &newComponentData[int(i * SizeOfComponentsBlock)] };

		//SizeType accumulateOffset{};
		for (const auto& compID : Types)
		{
			const SharedPtr<BaseComponent> thisComponent{ world.GetComponentTrait(compID) };
			const SizeType componentOffset{ ComponentOffsets.at(compID) };
			thisComponent->MoveData(startAddress + componentOffset, endAddress + componentOffset);
		}
	}

	return newComponentData;
}

void powe::Archetype::BuryBlock(WorldEntity& world,int index) const
{
	for (int i = index; i < int(GameObjectIds.size() - 1); ++i)
	{
		RawByte* fromAddress{ &ComponentData[int((i + 1) * SizeOfComponentsBlock)] };
		RawByte* toAddress{ &ComponentData[int(i * SizeOfComponentsBlock)] };

		for (const ComponentTypeID componentTypeId : Types)
		{
			const SizeType offset{ComponentOffsets.at(componentTypeId)};
			const SharedPtr<BaseComponent> thisComponent{ world.GetComponentTrait(componentTypeId) };
			thisComponent->MoveData(fromAddress + offset, toAddress + offset);
		}

		// A little bit unsafe
		--world.GetRefGameObjectRecord(GameObjectIds[i + 1]).IndexInArchetype;
	}
}

void powe::Archetype::CleanUp(const WorldEntity& world) const
{
	for (const GameObjectID gameObjectId : GameObjectIds)
	{
		GameObjectRecord gbRecord{};
		if(world.GetGameObjectRecords(gameObjectId, gbRecord))
		{
			RawByte* sourceAddress{ &ComponentData[int(gbRecord.IndexInArchetype * SizeOfComponentsBlock)] };
			for (const ComponentTypeID componentTypeId : Types)
			{
				const SizeType offset{ ComponentOffsets.at(componentTypeId) };
				const SharedPtr<BaseComponent> thisComponent{ world.GetComponentTrait(componentTypeId) };
				thisComponent->DestroyData(sourceAddress + offset);
			}
		}
	}
}


void powe::Archetype::AllocateComponentData(SizeType newSize,const WorldEntity& world)
{
	const SharedPtr<RawByte[]> newComponentData{ SharedPtr<RawByte[]>{new RawByte[newSize]{}} };

	for (int i = 0; i < int(GameObjectIds.size()); ++i)
	{
		RawByte* startAddress{ &ComponentData[int(i * SizeOfComponentsBlock)] };
		RawByte* endAddress{ &newComponentData[int(i * SizeOfComponentsBlock)] };

		//SizeType accumulateOffset{};
		for (const auto& compID : Types)
		{
			const SharedPtr<BaseComponent> thisComponent{ world.GetComponentTrait(compID) };
			const SizeType componentOffset{ComponentOffsets.at(compID)};
			thisComponent->MoveData(startAddress + componentOffset, endAddress + componentOffset);
		}
	}

	ComponentData = newComponentData;
	TotalAllocatedData = newSize;
}
