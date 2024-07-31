#include "pch.h"
#include "Clock.h"

powe::Clock::Clock()
	: m_CurrentFrame()
	, m_EndFrame()
	, m_DeltaTime()
	, m_TotalElapsedTime()
{
	m_EndFrame = std::chrono::high_resolution_clock::now();
}

void powe::Clock::Start()
{
	using seconds = std::chrono::duration<float>;
	m_CurrentFrame = std::chrono::high_resolution_clock::now();

	m_DeltaTime = std::chrono::duration_cast<seconds>(m_CurrentFrame - m_EndFrame).count();
	m_TotalElapsedTime += m_DeltaTime;
}

void powe::Clock::End()
{
	m_EndFrame = m_CurrentFrame;
}

void powe::Clock::ResetTime()
{
	m_EndFrame = std::chrono::high_resolution_clock::now();
}
