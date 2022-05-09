#include "pch.h"
#include "BaseComponent.h"

std::atomic<powe::ComponentTypeID> powe::BaseComponent::m_ComponentIdCounter{};

SharedPtr<powe::SparseComponent> powe::SparseComponent::GetStatic()
{
	static SharedPtr<SparseComponent> inst{ SharedPtr<SparseComponent>(new SparseComponent()) };
	return inst;
}
