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
