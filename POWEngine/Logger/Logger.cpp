#include "pch.h"
#include "Logger.h"

#include "Logger/Console/ConsoleLogger.h"
#include "Core/Memory/AllocatorContext.h"

using namespace powe;

Logger::Logger()
    : IModule("Logger")
{
    const AllocatorContext context{AllocatorScope::Logger};
    auto* upStream{context.GetResource()};

    m_LogSubsystem = AllocateUnique<ConsoleLogger>(upStream);
}

void Logger::OnCreate(ModulesManager*)
{
}

void Logger::OnExit(ModulesManager*)
{
}