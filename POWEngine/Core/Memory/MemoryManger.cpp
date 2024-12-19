#include "pch.h"
#include "MemoryManager.h"
#include "Core/CustomTypes.h"
#include "Core/Memory/Allocator.h"

using namespace powe;

void powe::MemoryManager::Init(const SharedPtr<PMRResource>& memResource)
{   
    auto instance{ MemoryManager::Get() };
    instance->m_ManagerAllocator = memResource.get();
    instance->m_AllocatorMap = UnOrderedMap<std::string, SharedPtr<PMRResource>>(memResource.get());
}

PMRResource* powe::MemoryManager::GetAllocator(std::string_view name) const noexcept
{
    if (m_AllocatorMap.find(name.data()) != m_AllocatorMap.end())
    {
        return m_AllocatorMap.at(name.data()).get();
    }

    return std::pmr::get_default_resource();
}

powe::PMRResource* powe::MemoryManager::NewAllocator(std::string_view name) noexcept
{
    if(m_AllocatorMap.find(name.data()) != m_AllocatorMap.end())
    {
        return m_AllocatorMap.at(name.data()).get();
    }

    SharedPtr<PMRResource> allocator{AllocateShared<TrackableAllocator>(m_ManagerAllocator, m_ManagerAllocator)};
    m_AllocatorMap[name.data()] = allocator;
    return allocator.get();
}

void powe::MemoryManager::RegisterAllocator(std::string_view name, PMRResource* allocator) noexcept
{
    m_AllocatorMap[name.data()] = SharedPtr<PMRResource>(allocator);
}

void powe::MemoryManager::RegisterAllocator(std::string_view name, SharedPtr<PMRResource> allocator) noexcept
{
    m_AllocatorMap[name.data()] = allocator;
}

