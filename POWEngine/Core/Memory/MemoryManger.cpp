#include "pch.h"
#include "MemoryManager.h"

#include "Core/Memory/Allocator.h"

using namespace powe;

void powe::MemoryManager::Init(const SharedPtr<PMRResource>& memResource)
{   
    auto* instance{ MemoryManager::Get() };
    instance->m_DefaultResource = memResource;
    instance->m_AllocatorMap = UnOrderedMap<std::string, SharedPtr<PMRResource>>(memResource.get());
}

powe::SharedPtr<powe::PMRResource> powe::MemoryManager::GetAllocator(std::string_view name) const
{
    if (m_AllocatorMap.find(name.data()) != m_AllocatorMap.end())
    {
        return m_AllocatorMap.at(name.data());
    }
    return nullptr;
}

powe::SharedPtr<powe::PMRResource> powe::MemoryManager::NewAllocator(std::string_view name)
{
    if(m_AllocatorMap.find(name.data()) != m_AllocatorMap.end())
    {
        return m_AllocatorMap.at(name.data());
    }

    SharedPtr<PMRResource> allocator{std::allocate_shared<TrackableAllocator>(m_DefaultResource)};
    m_AllocatorMap[name.data()] = allocator;
    return allocator;
}

void powe::MemoryManager::RegisterAllocator(std::string_view name, PMRResource* allocator)
{
    m_AllocatorMap[name.data()] = std::make_shared<PMRResource>(allocator);
}

void powe::MemoryManager::RegisterAllocator(std::string_view name, SharedPtr<PMRResource> allocator)
{
    m_AllocatorMap[name.data()] = allocator;
}

