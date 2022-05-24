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

void powe::Archetype::BuryBlock(const WorldEntity& world,int index) const
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

	TotalAllocatedData = newSize;
}
