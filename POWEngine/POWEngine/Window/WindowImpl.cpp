#include "pch.h"
#include "WindowImpl.h"

powe::WindowImpl::WindowImpl(uint32_t width, uint32_t height, const std::string& title)
	: m_WndMessages()
	, m_HWMessages()
	, m_MousePosLastPoll()
{
	UNREF_PARAM(width);
	UNREF_PARAM(height);
	UNREF_PARAM(title);
}

powe::WindowImpl::WindowImpl(uint32_t width, uint32_t height, const std::string& title, const OtherWindowParams& others)
	: m_WndMessages()
	, m_HWMessages()
	, m_MousePosLastPoll()
{
	UNREF_PARAM(width);
	UNREF_PARAM(height);
	UNREF_PARAM(title);
	UNREF_PARAM(others);
}

powe::WindowImpl::~WindowImpl() = default;
