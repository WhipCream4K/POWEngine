#include "pch.h"
#include "WindowImpl.h"

powe::WindowImpl::WindowImpl(uint32_t , uint32_t , const std::string& )
	: m_WndMessages()
	, m_HWMessages()
	, m_MousePosLastPoll()
{
}

powe::WindowImpl::WindowImpl(uint32_t , uint32_t , const std::string& , const OtherWindowParams& )
	: m_WndMessages()
	, m_HWMessages()
	, m_MousePosLastPoll()
{
}

powe::WindowImpl::~WindowImpl() = default;
