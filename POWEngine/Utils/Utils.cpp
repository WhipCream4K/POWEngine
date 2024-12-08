#include "pch.h"
#include "Utils.h"

#include "Core/Application/Application.h"
#include "Core/Memory/MemoryManager.h"

bool powe::IsInAppMainThread() noexcept
{ 
    return Application::Get().IsInMainThread();
}

powe::PMRResource* powe::GetAppResource() noexcept
{
    return MemoryManager::Get()->GetAllocator("Application");
}