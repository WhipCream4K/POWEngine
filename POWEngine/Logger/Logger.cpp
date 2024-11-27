#include "pch.h"
#include "Logger.h"

#include "Logger/Console/ConsoleLogger.h"
#include "Core/Memory/MemoryManager.h"

using namespace powe;

Logger::Logger()
    : IModule("Logger")
    , m_LogSubsystem(AllocateUnique<ConsoleLogger>(GetResource().get()))
{
}

void Logger::OnStartUp(ModulesManager*)
{
}

void Logger::OnExit(ModulesManager*)
{
}

SharedPtr<PMRResource> Logger::GetResource() const noexcept
{ 
    const auto loggerResc{MemoryManager::Get()->GetAllocator(GetName())};
    if(!loggerResc)
        return GetAppResource();

    return loggerResc;
}