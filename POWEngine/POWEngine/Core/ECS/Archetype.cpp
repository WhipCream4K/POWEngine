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
		RawByte* startAddress{ &other.ComponentData[int(i * SizeOfComponentsBlock)] };
		RawByte* endAddress{ &newComponentData[int(i * SizeOfComponentsBlock)] };

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

void powe::Archetype::AllocateComponentData(SizeType newSize,const WorldEntity& world)
{
	const SharedPtr<RawByte[]> newComponentData{ SharedPtr<RawByte[]>{new RawByte[newSize]{}} };

	for (int i = 0; i < int(GameObjectIds.size()); ++i)
	{
		RawByte* startAddress{ &ComponentData[int(i * SizeOfComponentsBlock)] };
		RawByte* endAddress{ &newComponentData[int(i * SizeOfComponentsBlock)] };

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
