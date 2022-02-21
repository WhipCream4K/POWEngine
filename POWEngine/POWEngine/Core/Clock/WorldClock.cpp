#include "pch.h"
#include "WorldClock.h"

powe::WorldClock::WorldClock()
	: m_CurrentFrame()
	, m_EndFrame()
	, m_DeltaTime()
	, m_TotalElapsedTime()
{
}

void powe::WorldClock::Start()
{
	using seconds = std::chrono::duration<float>;
	m_CurrentFrame = std::chrono::high_resolution_clock::now();

	m_DeltaTime = std::chrono::duration_cast<seconds>(m_CurrentFrame - m_EndFrame).count();
	m_TotalElapsedTime += m_DeltaTime;
}

void powe::WorldClock::End()
{
	m_EndFrame = m_CurrentFrame;
}
