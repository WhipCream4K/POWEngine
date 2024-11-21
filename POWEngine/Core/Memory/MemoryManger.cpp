#include "pch.h"
#include "MemoryManager.h"

powe::PMRResource* powe::MemoryManager::GetAllocator(std::string_view name) const
{
    if (m_AllocatorMap.find(name.data()) != m_AllocatorMap.end())
    {
        return m_AllocatorMap.at(name.data()).get();
    }
    return nullptr;
}

powe::PMRResource* powe::MemoryManager::NewAllocator(std::string_view name)
{
    if (m_AllocatorMap.find(name.data()) != m_AllocatorMap.end())
    {
        return m_AllocatorMap[name.data()].get();
    }
    return nullptr;
}

void powe::MemoryManager::RegisterAllocator(std::string_view name, PMRResource* allocator)
{
    m_AllocatorMap[name.data()] = std::make_shared<PMRResource>(allocator);
}

powe::SharedPtr<powe::PMRResource> powe::MemoryManager::GetSharedAllocator(std::string_view name) const
{
    if (m_AllocatorMap.find(name.data()) != m_AllocatorMap.end())
    {
        return m_AllocatorMap.at(name.data());
    }
    return nullptr;
}

void powe::MemoryManager::RegisterAllocator(std::string_view name, SharedPtr<PMRResource> allocator)
{
    m_AllocatorMap[name.data()] = allocator;
}