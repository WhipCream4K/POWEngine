#include "pch.h"
#include "WindowImpl.h"

powe::WindowImpl::WindowImpl(uint32_t , uint32_t , const std::string& )
{
}

powe::WindowImpl::WindowImpl(uint32_t , uint32_t , const std::string& , const OtherWindowParams& )
{
}

std::mutex& powe::WindowImpl::GetWindowMutex()
{
    return m_ClassMutex;
}

powe::WindowImpl::~WindowImpl() = default;
