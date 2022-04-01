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
	//if (!ComponentData)
	//	ComponentData = std::shared_ptr<RawByte[]>{ new RawByte[other.TotalAllocatedData]{} };

	SharedPtr<RawByte[]> newComponentData{ SharedPtr<RawByte[]>{new RawByte[other.TotalAllocatedData]{}} };

	for (int i = 0; i < int(GameObjectIds.size()); ++i)
	{
		RawByte* startAddress{ &other.ComponentData[SizeType(i * SizeOfComponentsBlock)] };
		RawByte* endAddress{ &newComponentData[SizeType(i * SizeOfComponentsBlock)] };

		SizeType accumulateOffset{};
		for (const auto& comID : Types)
		{
			const SharedPtr<BaseComponent> thisComponent{ world.GetComponentMap().at(comID) };
			thisComponent->MoveData(startAddress + accumulateOffset, endAddress + accumulateOffset);
			accumulateOffset += thisComponent->GetSize();
		}
	}

	return newComponentData;
}

//void powe::Archetype::CopyComponentData(const WorldEntity& world,const Archetype& other, int indexInArchetype)
//{
//	//SizeType accumulatedOffset{};
//	//RawByte* startAddress{ &other.ComponentData[indexInArchetype * other.SizeOfComponentsBlock] };
//	//RawByte* endAddress{};
//
//	//for (const auto& compID : Types)
//	//{
//	//	accumulatedOffset += 
//	//}
//}

void powe::Archetype::AllocateComponentData(SizeType newSize,const WorldEntity& world)
{
	const SharedPtr<RawByte[]> newComponentData{ SharedPtr<RawByte[]>{new RawByte[newSize]{}} };

	for (int i = 0; i < int(GameObjectIds.size()); ++i)
	{
		RawByte* startAddress{ &ComponentData[SizeType(i * SizeOfComponentsBlock)] };
		RawByte* endAddress{ &newComponentData[SizeType(i * SizeOfComponentsBlock)] };

		SizeType accumulateOffset{};
		for (const auto& comID : Types)
		{
			const SharedPtr<BaseComponent> thisComponent{ world.GetComponentMap().at(comID) };
			thisComponent->MoveData(startAddress + accumulateOffset, endAddress + accumulateOffset);
			accumulateOffset += thisComponent->GetSize();
		}
	}

	TotalAllocatedData = newSize;
}